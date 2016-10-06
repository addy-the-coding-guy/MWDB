function generate_color_histogram(p_directory, p_resolution, p_bin_size, p_out_file)

    directory_path = p_directory;
    resolution = p_resolution;
    bin_size = p_bin_size;
    out_file = p_out_file;
    % check if the directory is valid
        if ~exist(directory_path, 'dir')
            errorMessage = sprintf('Error: %s does not exist.', directory_path);
            uiwait(warndlg(errorMessage));
            return;

        else
            % split the video files in the directory to corresponding frames
            split_frames(directory_path);
            % generate the histograms for each frame
            generate_block_histogram(directory_path, resolution, bin_size, out_file);
        end
end

