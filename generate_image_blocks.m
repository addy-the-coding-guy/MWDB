function cell_array = generate_image_blocks(p_image_path, p_number_of_rows, p_number_of_cols)
    % Divides an image up into blocks by using mat2cell().

    number_of_rows = p_number_of_rows;
    number_of_cols = p_number_of_cols;
    
    image_matrix = imread(p_image_path);
    
    [rows, columns] = size(image_matrix);

    % get number of rows and columns in the image_matrix
    % calculate the actual number of rows and cols for each block
    number_of_rows = floor(rows / number_of_rows);
    number_of_cols = floor(columns / number_of_cols);
    
    % Most will be blockSizeR but there may be a remainder amount of less than that.
    wholeBlockRows = floor(rows / number_of_rows);
    blockVectorR = [number_of_rows * ones(1, wholeBlockRows), rem(rows, number_of_rows)];
    % Figure out the size of each block in columns. 
    wholeBlockCols = floor(columns / number_of_cols);
    blockVectorC = [number_of_cols * ones(1, wholeBlockCols), rem(columns, number_of_cols)];

    % Create the cell array  
    % Each cell (except for the remainder cells at the end of the image)
    % in the array contains a blockSizeR by blockSizeC by 3 color array.
    % This line is where the image is actually divided up into blocks.
    cell_array = mat2cell(image_matrix, blockVectorR, blockVectorC);
    
end