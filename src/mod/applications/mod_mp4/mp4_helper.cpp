/*

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.

The Original Code is MP4 Helper Library to the Freeswitch MP4 Module.

The Initial Developer of the Original Code is 
Paulo Rogério Panhoto <paulo@voicetechnology.com.br>.
Portions created by Paulo Rogério Panhoto are 
Copyright (C) 2010, Voice Technology Ind. e Com. Ltda. All Rights Reserved.

*/

#include "mp4_helper.hpp"

namespace MP4
{
	
	Context::Context(const char * file)
	{
		open(file);
	}
	
	Context::~Context()
	{
		close();
	}

	bool Context::open(const char * file)
	{
		fh = MP4Read(file, 0);
		return fh != MP4_INVALID_FILE_HANDLE && getTracks();
	}

	void Context::close()
	{
		if(!isOpen())
			return;
		MP4Close(fh);
	}

	bool Context::getTracks()
	{
		int i = 0;
		bool audioTrack = false, videoTrack = false;

		if(!isOpen()) return false;

		while(true)
		{
			TrackProperties track;
			if((track.hint = MP4FindTrackId(fh, i++, MP4_HINT_TRACK_TYPE, 0)) == MP4_INVALID_TRACK_ID)
				break;

			MP4GetHintTrackRtpPayload(fh, track.hint, &track.codecName, &track.payload, NULL, NULL);

			track.track = MP4GetHintTrackReferenceTrackId(fh, track.hint);
			if(track.track == MP4_INVALID_TRACK_ID) continue;
			track.clock = MP4GetTrackTimeScale(fh, track.hint);

			if(!strcmp(MP4GetTrackType(fh, track.track), MP4_AUDIO_TRACK_TYPE))
			{
				audioTrack = true;

				if(!strncmp(track.codecName, "PCM", 3))
					track.packetLength = 20;
				else
					track.packetLength = track.clock = 0;

				audio = track;
			} else if(!strcmp(MP4GetTrackType(fh, track.track), MP4_VIDEO_TRACK_TYPE))
			{
				videoTrack = true;

				const char * sdp = MP4GetHintTrackSdp(fh, track.hint);
				const char * fmtp = strstr(sdp, "fmtp");

				if(fmtp)
				{
					// finds beginning of 'fmtp' value;
					for(fmtp += 5; *fmtp != ' '; ++fmtp);
					++fmtp;

					const char * eol = fmtp;
					for(;*eol != '\r' && *eol != '\n'; ++eol);
					video.fmtp = std::string(fmtp, eol);
				}
				video.track = track;
			}
		}

		return audioTrack && videoTrack;
	}

	bool Context::getVideoPacket(void * buffer, u_int & size, u_int & ts)
	{
		return getPacket(video.track.hint, video.track.runtime, true, buffer, size, ts);
	}

	bool Context::getAudioPacket(void * buffer, u_int & size, u_int & ts)
	{
		return getPacket(audio.hint, audio.runtime, false, buffer, size, ts);
	}

	bool Context::getPacket(MP4TrackId hint, RuntimeProperties & rt,
				bool header, void * buffer, u_int & size, u_int & ts)
	{
		if(rt.frame == 0 || rt.packet == rt.packetsPerFrame)
		{
			++rt.frame;
			if(!MP4ReadRtpHint(fh, hint, rt.frame, &rt.packetsPerFrame))
				return false;
			rt.packet = 0;
			rt.last_frame = MP4GetSampleTime(fh, hint, rt.frame);
		}

		ts = rt.last_frame;
		if(!MP4ReadRtpPacket(fh, hint, rt.packet, (u_int8_t **) &buffer, &size, 0, header, true))
			return false;
		++rt.packet;
		return true;
	}

}