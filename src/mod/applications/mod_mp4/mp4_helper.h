/*

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.

The Original Code is Video File Abstraction Library to Freeswitch

The Initial Developer of the Original Code is 
Paulo Rogério Panhoto <paulo@voicetechnology.com.br>.
Portions created by Paulo Rogério Panhoto are 
Copyright (C) 2010, Voice Technology Ind. e Com. Ltda. All Rights Reserved.

*/

#ifndef VIDEO_HELPER_H_
#define VIDEO_HELPER_H_

#include <mp4.h>

#ifndef TRUE
#define TRUE (1 == 1)
#endif

#ifndef FALSE
#define FALSE !TRUE
#endif

typedef struct
{
	u_int32_t frame; // sampleID
	u_int16_t packetsPerFrame;
	u_int16_t packet; // packetID
} RuntimeProperties;


typedef struct
{
	MP4TrackId hint;
	MP4TrackId track;
	
	char * codecName;
	u_int8_t payload;
	u_int32_t clock;
	u_int32_t packetLength; // packet Length in time (ms)
	
	RuntimeProperties runtime;
} TrackProperties;

typedef TrackProperties AudioProperties;

typedef struct
{
	TrackProperties base;

	char * fmtp;
} VideoProperties;

typedef struct
{
	MP4FileHandle fh;
	AudioProperties audio;

	VideoProperties video;
	
} VideoContext;

VideoContext * VideoOpen(const char * file);
void VideoClose(VideoContext * ctx);

// returns: TRUE = success, FALSE = failure.
int VideoGetTracks(VideoContext * ctx);

// returns: TRUE = has more data, FALSE = end-of-stream or failure
int VideoGetVideoPacket(VideoContext * ctx, char * buffer, u_int * size);

// returns: TRUE = has more data, FALSE = end-of-stream or failure
int VideoGetAudioPacket(VideoContext * ctx, char * buffer, u_int * size);

#endif
