function generate_sift_descriptors(p_directory, p_resolution, p_out_file)
    % root directory of the video files
    root_directory = p_directory;
    
    % rows and cols for splitting the image into blocks
    number_of_rows = p_resolution;
    number_of_cols = p_resolution;
    
    % name of the output file
    output_file = p_out_file;
    
    % get all the files in the given root directory
    %files = dir(root_directory);
    temporary_files = fullfile(root_directory,'temp*');
    files = dir(temporary_files);
    
    % flag to extract only directories from the root_directory
    % dir_flag = [files.isdir];
    
    % extract all the folders in the root directory
    % sub_folders = files(dir_flag);
    
    % count all the temporary folders in the root directory
    % number_of_folders = numel(sub_folders);
    number_of_folders = numel(files);
        
    % get the histograms on all the images in the folder
    for i = 1:number_of_folders        
        
            % construct the path for the current temporary folder
            temp_folder_path = strcat(root_directory, '\', files(i).name);
            
            
            % sort the images according to number
            image_list = dir(fullfile(temp_folder_path, '\*.jpg'));
            image_files = {image_list.name};
            str  = sprintf('%s#', image_files{:});
            num  = sscanf(str, 'image_%d.jpg#');
            [dummy_row, index] = sort(num);

            % cell array containing sorted image files
            image_files = image_files(index);
            
            % start the output formatting here
            fileId = fopen(output_file,'at');
                fprintf(fileId, '\nSIFT VECTORS FOR VIDEO %d', i); 
                fprintf(fileId, '\n=============================');
                fclose(fileId);
                
            for j = 1:length(image_files)
                image_path = strcat(temp_folder_path, '\', image_files{1});
                
                % split the image into blocks
                ca = generate_image_blocks(image_path, number_of_rows, number_of_cols);
                
                for m = 1:p_resolution
                    for n = 1:p_resolution
                        b = ca{m,n};
                        if m==1 && n==1
                            block_number = 1;
                        elseif m==1 && n==2
                            block_number = 2;
                        elseif m==2 && n == 1
                            block_number = 3;
                        else
                            block_number = 4;
                        end
                        
                        % generate the sift descriptors
                        % [count, x] = imhist(b);
                        [frames, descriptors] = sift( b, 'Verbosity', 1 ) ;
                        
                        % actual output to the output file
                        % loop and print sift vectors on k frames
                        fileId = fopen(output_file,'at+');
                        
                       for frame_number = 1: size(frames,2)
                        
                            fprintf(fileId, '\nvideo number: %d', i);

                            fprintf(fileId, '\nframe number: %d', j);

                            fprintf(fileId, '\nblock number: %d', block_number);

                            fprintf(fileId, '\nsift vector: ');
                            
                            % center(x); center(y); scale; a1...a128
                            fprintf(fileId, '< %d, %d, %d, ',frames(1, frame_number), frames(2, frame_number), frames(3, frame_number));
                                                        
                            fprintf(fileId, '%d, ',descriptors(1:127, frame_number));
                            
                            fprintf(fileId, '%d', descriptors(128, frame_number));
                            
                            fprintf(fileId, ' >');
                       end
                        fclose(fileId);
                    end
                end
            end
    end 
end