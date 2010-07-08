/*

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.

The Original Code is MP4 Abstraction Library to the Freeswitch MP4 Module.

The Initial Developer of the Original Code is 
Paulo Rogério Panhoto <paulo@voicetechnology.com.br>.
Portions created by Paulo Rogério Panhoto are 
Copyright (C) 2010, Voice Technology Ind. e Com. Ltda. All Rights Reserved.

*/

#include "mp4_helper.h"

static int VideoGetPacket(MP4FileHandle fh, MP4TrackId hint, RuntimeProperties * rt,
													int header, char * buffer, int * size);


VideoContext * VideoOpen(const char * file)
{
	MP4FileHandle fh = MP4Read(file, 0);
	if(fh == MP4_INVALID_FILE_HANDLE)
		return NULL;
	
	VideoContext * vc = malloc(sizeof(VideoContext));
	memset(vc, 0, sizeof vc);
	vc->fh = fh;
	return vc;
}

void VideoClose(VideoContext * context)
{
	if(context == NULL)
		return;
	MP4Close(context->fh);
	
	free(context->video.fmtp);
	free(context);
}

int VideoGetTracks(VideoContext * context)
{
	if(context == NULL) return 0;
	
	TrackProperties track;
	int i=0;
	int audio = 0, video = 0;
	
	while((track.hint = MP4FindTrackId(context->fh, i++, MP4_HINT_TRACK_TYPE, 0)) != MP4_INVALID_TRACK_ID)
	{

		MP4GetHintTrackSDPPayload(context->fh, track.hint, &track.codecName, &track.payload, &track.packetLength, NULL);

		track.track = MP4GetHintTrackReferenceTrackId(context->fh, track.hint);
		if(track.track == MP4_INVALID_TRACK_ID) continue;
		track.clock = MP4GetTrackTimeScale(context->fh, track.hint);

		if(!strcmp(MP4GetTrackType(context->fh, track.track), MP4_AUDIO_TRACK_TYPE))
		{
			audio = 1;
			memcpy(&context->audio, &track, sizeof(TrackProperties));

			if(!strncmp(context->audio.codecName, "PCM", 3))
				track.packetLength = 20;
			else
				track.packetLength = track.clock = 0;
		} else if(!strcmp(MP4GetTrackType(context->fh, track.track), MP4_VIDEO_TRACK_TYPE))
		{
			video = 1;
			memcpy(&context->video.base, &track, sizeof(TrackProperties));

			
			const char * sdp = MP4GetHintTrackSdp(context->fh, context->video.base.hint);

			const char * fmtp = strstr(sdp, "fmtp");
			if(fmtp)
			{
				// finds beginning of 'fmtp' value;
				 
				for(fmtp += 5; *fmtp != ' '; ++fmtp);
				++fmtp;

				const char * eol = fmtp;
				for(;*eol != '\r' && *eol != '\n'; ++eol);
				context->video.fmtp = malloc(1 + eol - fmtp);
				strncpy(context->video.fmtp, fmtp, eol - fmtp);
				context->video.fmtp[eol - fmtp] = 0;
			}
		}
	}
	
	return audio && video;
}

// returns: 1 = has more data, 0 = end-of-stream or failure
int VideoGetVideoPacket(VideoContext * ctx, char * buffer, int * size)
{
	return VideoGetPacket(ctx->fh, ctx->video.base.hint, &ctx->video.base.runtime, TRUE, buffer, size);
}

// returns: 1 = has more data, 0 = end-of-stream or failure
int VideoGetAudioPacket(VideoContext * ctx, char * buffer, int * size)
{
	return VideoGetPacket(ctx->fh, ctx->audio.hint, &ctx->audio.runtime, FALSE, buffer, size);
}

static int VideoGetPacket(MP4FileHandle fh, MP4TrackId hint, RuntimeProperties * rt,
													int header, char * buffer, int * size)
{
	if(rt->frame == 0 || rt->packet == rt->packetsPerFrame)
	{
		if(!MP4ReadRtpHint(fh, hint, ++rt->frame, &rt->packetsPerFrame))
			return FALSE;
		rt->packet = 0;
	}

	if(!MP4ReadRtpPacket(fh, hint, rt->packet++, &buffer, size, 0, header, TRUE))
		return FALSE;
	return TRUE;
}
