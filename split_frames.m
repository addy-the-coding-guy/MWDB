function split_frames(p_directory)
    % split the video file into image frames
    % check if the directory exists
    directory_path = p_directory;

    % all videos with the extension .mp4
    all_video_files = '*.mp4';

    % full path of all video files in the given directory
    full_directory_path = fullfile(directory_path, all_video_files);

    % get info of all the video files in the directory
    video_files = dir(full_directory_path)

    % sort the video files according to video file name here

    % count number of videos in the directory
    num_of_videos = length(video_files(not([video_files.isdir])));

    % keep track of the current folder
    root_directory = directory_path;

    %split each video into corresponding frames iteratively
    for i = 1:num_of_videos
        % create a temporary folder with video number as directory name
        % folder name keeps track of video number 'i'
        folder_name = strcat('temp_',num2str(i));
        temp_folder_name = fullfile(root_directory,folder_name);

        if ~exist(temp_folder_name,'dir')
            % create if the video directory does not exist
            mkdir(temp_folder_name);
            % temp folder name consists the full path constructed from root
            % path
            temp_directory = temp_folder_name;
        else
            % if the directory exists, that is the temp_directory
            temp_directory = temp_folder_name;
        end
        video_path = fullfile(root_directory, video_files(i).name)
        vid = VideoReader(video_path);
        numFrames = vid.NumberOfFrames;
        n = numFrames;

        % split video to frames and write to the temporary directory
        for j = 1:n
            frames = read(vid,j);

            image_number = int2str(j);

            % write the grayscale frames to the temporary folder
            imwrite(rgb2gray(frames),[temp_directory,'\image_', image_number,'.jpg']);

        end

    end
end
