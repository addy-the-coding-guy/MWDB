// ffmpegTest.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "stdio.h"
#include "conio.h"

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include <locale>
#include <sstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

// Globals
char input_directory[100];
char output_file[100];
int resolution = 0;
std::string root_directory;
std::string output_file_string;
int v_width = 160;
int v_height = 120;


extern "C"
{
#include <libavutil/motion_vector.h>
#include <libavformat/avformat.h>
# pragma comment (lib, "avformat.lib")
}
static AVFormatContext *fmt_ctx = NULL;
static AVCodecContext *video_dec_ctx = NULL;
static AVStream *video_stream = NULL;
static const char *src_filename = NULL;

static int video_stream_idx = -1;
static AVFrame *frame = NULL;
static AVPacket pkt;
static int video_frame_count = 0;

static int decode_packet(int *got_frame, int cached, int video_number, int r)
{
	int decoded = pkt.size;

	*got_frame = 0;

	if (pkt.stream_index == video_stream_idx) {
		int ret = avcodec_decode_video2(video_dec_ctx, frame, got_frame, &pkt);
		if (ret < 0) {
			//TODO : Fix the below line
			//fprintf(stderr, "Error decoding video frame (%s)\n", av_err2str(ret));
			return ret;
		}

		if (*got_frame) {
			int i;
			AVFrameSideData *sd;

			video_frame_count++;
			sd = av_frame_get_side_data(frame, AV_FRAME_DATA_MOTION_VECTORS);
			if (sd) {
				const AVMotionVector *mvs = (const AVMotionVector *)sd->data;
				for (i = 0; i < sd->size / sizeof(*mvs); i++) {
					const AVMotionVector *mv = &mvs[i];

					// TODO: MODIFY BELOW LINES TO OUTPUT TO AN OUTPUT DIRECTORY
					// ADD AN ARGUMENT TO THE FUNCTION THAT TAKES THE OUTPUT DIRECTORY
					//printf("%d %d %d %d %d %d %d %d 0x%\n",PRIx64,"\n",
					// map the block number here..

					int block_size_x = v_width / r;
					int block_size_y = v_height / r;
					int *ranges_x = new int[r];
					int *ranges_y = new int[r];
					for (i = 1; i <= r; i++) {
						ranges_x[i - 1] = block_size_x * i;
						ranges_y[i - 1] = block_size_y * i;
					}

					int block_number = 1;
					if (mv->src_x >= 0 && mv->src_x < ranges_x[0]) {
						if (mv->src_y >= 0 && mv->src_y < ranges_y[0]) {
							block_number == 1;
						}
						else if (mv->src_y >= ranges_y[0] && mv->src_y <= ranges_y[1]) {
							block_number = 3;
						}
					}

					if (mv->src_x >= ranges_x[0] && mv->src_x <= ranges_x[1]) {
						if (mv->src_y >= 0 && mv->src_y < ranges_y[0]) {
							block_number = 2;
						}
						else if (mv->src_y >= ranges_y[0] && mv->src_y <= ranges_y[1]) {
							block_number = 4;
						}
					}

					std::string output_f_path_root(input_directory);
					std::string output_f_name(output_file);
					std::string output_file_cpath = output_f_path_root + "\\" + output_f_name;
					FILE* output_file_pointer = fopen(output_file_cpath.c_str(), "a+");
					fprintf(output_file_pointer, "Video Number, Frame Number, Block Number, Source, Width, Height, Source->x, Source->Y, Dest->X, Dest->Y, flags \n");
					fprintf(output_file_pointer, "     %d      ,    %d      ,       %d     ,   %d  ,  %d  ,   %d  ,    %d    ,    %d    ,   %d   ,   %d   ,   %d  \n",
						i, video_frame_count, block_number, mv->source,
						mv->w, mv->h, mv->src_x, mv->src_y,
						mv->dst_x, mv->dst_y, mv->flags);
					fprintf(output_file_pointer, "--------------------------------------------------------------------------------------------------------------- \n");
					fclose(output_file_pointer);
				}
			}
		}
	}

	return decoded;
}

static int open_codec_context(int *stream_idx,
	AVFormatContext *fmt_ctx, enum AVMediaType type)
{
	int ret;
	AVStream *st;
	AVCodecContext *dec_ctx = NULL;
	AVCodec *dec = NULL;
	AVDictionary *opts = NULL;

	ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
	if (ret < 0) {
		fprintf(stderr, "Could not find %s stream in input file '%s'\n",
			av_get_media_type_string(type), src_filename);
		return ret;
	}
	else {
		*stream_idx = ret;
		st = fmt_ctx->streams[*stream_idx];

		/* find decoder for the stream */
		dec_ctx = st->codec;
		dec = avcodec_find_decoder(dec_ctx->codec_id);
		if (!dec) {
			fprintf(stderr, "Failed to find %s codec\n",
				av_get_media_type_string(type));
			return AVERROR(EINVAL);
		}

		/* Init the video decoder */
		av_dict_set(&opts, "flags2", "+export_mvs", 0);
		if ((ret = avcodec_open2(dec_ctx, dec, &opts)) < 0) {
			fprintf(stderr, "Failed to open %s codec\n",
				av_get_media_type_string(type));
			return ret;
		}
	}

	return 0;
}

// Convert a std::string to a wide character
// The following is a widening conversion
wchar_t* cast_to_wide_char(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

// Convert a wide char to string
// The following is a narrowing conversion
std::string cast_to_string(const wchar_t *s, char dfault = '?', const std::locale& loc = std::locale())
{
	std::ostringstream stream;

	while (*s != L'\0') {
		stream << std::use_facet< std::ctype<wchar_t> >(loc).narrow(*s++, dfault);
	}
	return stream.str();
}

// Function to get all the video files in a given directory
std::vector<std::string> get_video_file_names(std::string directory) {

	std::vector<std::string> file_listing;
	WIN32_FIND_DATA file_info;
	HANDLE file_handle;

	if (!((file_handle = FindFirstFile(cast_to_wide_char(directory.c_str()), &file_info)) == INVALID_HANDLE_VALUE)) {
		while (FindNextFile(file_handle, &file_info)) {

			file_listing.push_back(cast_to_string(file_info.cFileName));
		}
	}

	FindClose(file_handle);
	return file_listing;
}


int main(int argc, char **argv)
{

	// User input to get the input directory
	std::cout << "Enter the path of the input directory:";
	std::cin.getline(input_directory, sizeof(input_directory));

	std::cout << "Enter the output file name(The output will be written to the video directory):";
	std::cin.getline(output_file, sizeof(output_file));

	std::cout << "Enter the resolution for the video blocks:";
	std::cin >> resolution;

	std::vector<std::string> video_files;

	// Check if the directory is valid
	struct stat info;

	if (stat(input_directory, &info) != 0) {
		std::cout << "cannot access:" << input_directory << std::endl;
		exit(1);
	}
	else if (info.st_mode & S_IFDIR) {

		// Get the video files in the directory
		// Store them to the array vide_file_names
		std::string video_file_names = (std::string)input_directory + "\\*.mp4";
		std::cout << "\nThe path of the video file names is:" << video_file_names << std::endl;
		video_files = get_video_file_names(video_file_names);
	}

	else {
		printf("%s is no directory\n", input_directory);
		exit(1);
	}


	int ret = 0, got_frame;

	for (int i = 0; i < video_files.size(); i++) {

		if (argc != 2) {
			fprintf(stderr, "Usage: %s <video>\n", argv[0]);
			exit(1);
		}

		// check if the directory has source files
		if (video_files.size() == 0) {
			std::cout << "Sorry! Couldn't find any video files in the directory!";
			break;
		}


		// Assign the video filename to src_filename
		std::string root_path(input_directory);
		std::string complete_input_path = root_path + "\\" + video_files[i];
		src_filename = complete_input_path.c_str();

		av_register_all();


		if (avformat_open_input(&fmt_ctx, src_filename, NULL, NULL) < 0) {
			fprintf(stderr, "Could not open source file %s\n", src_filename);
			exit(1);
		}

		if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
			fprintf(stderr, "Could not find stream information\n");
			exit(1);
		}

		if (open_codec_context(&video_stream_idx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
			video_stream = fmt_ctx->streams[video_stream_idx];
			video_dec_ctx = video_stream->codec;
		}

		av_dump_format(fmt_ctx, 0, src_filename, 0);

		if (!video_stream) {
			fprintf(stderr, "Could not find video stream in the input, aborting\n");
			ret = 1;
			goto end;
		}

		frame = av_frame_alloc();
		if (!frame) {
			fprintf(stderr, "Could not allocate frame\n");
			ret = AVERROR(ENOMEM);
			goto end;
		}

		printf("framenum,source,blockw,blockh,srcx,srcy,dstx,dsty,flags\n");

		/* initialize packet, set data to NULL, let the demuxer fill it */
		av_init_packet(&pkt);
		pkt.data = NULL;
		pkt.size = 0;

		/* read frames from the file */
		while (av_read_frame(fmt_ctx, &pkt) >= 0) {
			AVPacket orig_pkt = pkt;
			do {
				ret = decode_packet(&got_frame, 0, i, resolution);
				if (ret < 0)
					break;
				pkt.data += ret;
				pkt.size -= ret;
			} while (pkt.size > 0);
			av_packet_unref(&orig_pkt);
		}

		/* flush cached frames */
		pkt.data = NULL;
		pkt.size = 0;
		do {
			decode_packet(&got_frame, 1, i, resolution);
		} while (got_frame);


	end:
		avcodec_close(video_dec_ctx);
		avformat_close_input(&fmt_ctx);
		av_frame_free(&frame);
	}
	return ret < 0;

}
