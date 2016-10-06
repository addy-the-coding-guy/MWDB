function generate_sift_vectors(p_directory, p_resolution, p_out_file)

    directory_path = p_directory;
    resolution = p_resolution;
    out_file = p_out_file;
    % check if the directory is valid
        if ~exist(directory_path, 'dir')
            errorMessage = sprintf('Error: %s does not exist.', directory_path);
            uiwait(warndlg(errorMessage));
            return;

        else
            % split the video files in the directory to corresponding frames
            split_frames(directory_path);
            % generate the sift for each frame
            generate_sift_descriptors(directory_path, resolution, out_file);
        end
end

