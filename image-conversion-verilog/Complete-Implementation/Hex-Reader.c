module hex_file_reader (
  input clk,
  input rst,
  input startRead,
  output reg readDone,
  output reg [10:0] RED [0:imageSize-1],
  output reg [10:0] GREEN [0:imageSize-1],
  output reg [10:0] BLUE [0:imageSize-1]
);

  parameter ROWS = 985;
  parameter COLS = 16;
  parameter imageFile = "car.hex";

  localparam imageSize = ROWS * COLS;

  reg [6:0] totalImageMemory [0:imageSize-1];
  reg readDoneReg;

  integer localPos = 0;
  integer assignColour = 0;

  initial begin
    $readmemh(imageFile, totalImageMemory, 0, imageSize-1);
    readDoneReg = 0;
  end

  always @(posedge clk or posedge rst or posedge startRead) begin
    if (rst) begin
      // Reset logic here
      readDoneReg <= 0;
      localPos <= 0;
      assignColour <= 0;
    end else begin
      // Read logic
      if (startRead) begin
        if (localPos < imageSize) begin
          RED[assignColour] <= totalImageMemory[localPos];
          GREEN[assignColour] <= totalImageMemory[localPos + 1];
          BLUE[assignColour] <= totalImageMemory[localPos + 2];
          assignColour = assignColour + 1;
          localPos = localPos + 3;
        end else begin
          // All data read, set readDone
          readDoneReg <= 1;
        end
      end else begin
        // Reset readDone when not reading
        readDoneReg <= 0;
      end
    end
  end

  assign readDone = readDoneReg;

endmodule

module rgb_data_reader (
  input clk,
  input rst,
  input startRead,
  output reg readDone,
  output reg [10:0] RED [0:imageSize-1],
  output reg [10:0] GREEN [0:imageSize-1],
  output reg [10:0] BLUE [0:imageSize-1]
);

  parameter ROWS = 985;
  parameter COLS = 16;
  parameter imageFile = "car.hex";

  localparam imageSize = ROWS * COLS;

  // Wire to connect to the readDone signal in hex_file_reader
  wire readDone_wire;
  wire [10:0] RED_wire [0:imageSize-1];
  wire [10:0] GREEN_wire [0:imageSize-1];
  wire [10:0] BLUE_wire [0:imageSize-1];

  // Instantiate the hex_file_reader module
  hex_file_reader reader_inst (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .readDone(readDone_wire),
    .RED(RED_wire),
    .GREEN(GREEN_wire),
    .BLUE(BLUE_wire)
  );

  // Connect the readDone signal from hex_file_reader
  assign readDone = readDone_wire;

  // Connect the RED, GREEN, and BLUE signals
  assign RED = RED_wire;
  assign GREEN = GREEN_wire;
  assign BLUE = BLUE_wire;
  
endmodule

module grey_scale_conversion (
  input clk,
  input rst,
  input startRead,
  output reg readDone,
  output reg [10:0] greyScale [0:imageSize-1]
);

  parameter ROWS = 985;
  parameter COLS = 16;
  parameter imageFile = "car.hex";
  
  localparam imageSize = ROWS * COLS;

  integer localPos = 0;

  wire readDone_rgb;
  wire [10:0] RED_rgb [0:imageSize-1];
  wire [10:0] GREEN_rgb [0:imageSize-1];
  wire [10:0] BLUE_rgb [0:imageSize-1];

  // Instantiate the rgb_data_reader module
  rgb_data_reader reader_inst (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .readDone(readDone_rgb),
    .RED(RED_rgb),
    .GREEN(GREEN_rgb),
    .BLUE(BLUE_rgb)
  );

  // Connect the readDone signal from rgb_data_reader
  assign readDone = readDone_rgb;

  always @(posedge clk or posedge rst or posedge startRead) begin
    if (startRead) begin
      for (localPos = 0; localPos < imageSize; localPos = localPos + 1) begin
        // Grayscale conversion using the luminosity method
        greyScale[localPos] = (0.299 * RED_rgb[localPos]) + (0.587 * GREEN_rgb[localPos]) + (0.114 * BLUE_rgb[localPos]);
        // Grayscale conversion using the average method
        //greyScale[localPos] = (RED_rgb[localPos] + GREEN_rgb[localPos] + BLUE_rgb[localPos]) / 3;
      end
    end
  end

endmodule

module black_white_conversion (
  input clk,
  input rst,
  input startRead,
  output reg readDone,
  output reg [10:0] whiteBlack [0:imageSize-1]
);

  parameter ROWS = 985;
  parameter COLS = 16;
  parameter imageFile = "car.hex";
  
  parameter thresholdBlackWhite = 128; // This is half of 256 bit color scale. 
  
  localparam imageSize = ROWS * COLS;

  integer localPos = 0;

  wire readDone_rgb;
  wire [10:0] RED_rgb [0:imageSize-1];
  wire [10:0] GREEN_rgb [0:imageSize-1];
  wire [10:0] BLUE_rgb [0:imageSize-1];

  // Instantiate the rgb_data_reader module
  rgb_data_reader reader_inst (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .readDone(readDone_rgb),
    .RED(RED_rgb),
    .GREEN(GREEN_rgb),
    .BLUE(BLUE_rgb)
  );

  // Connect the readDone signal from rgb_data_reader
  assign readDone = readDone_rgb;

  always @(posedge clk or posedge rst or posedge startRead) begin
    if (startRead) begin
      for (localPos = 0; localPos < imageSize; localPos = localPos + 1) begin
        // Grayscale conversion using the luminosity method
        whiteBlack[localPos] = (0.299 * RED_rgb[localPos]) + (0.587 * GREEN_rgb[localPos]) + (0.114 * BLUE_rgb[localPos]);

        // Convert to black and white
        if (whiteBlack[localPos] > thresholdBlackWhite) begin
          whiteBlack[localPos] = 1'b1; // Set to white (1)
        end else begin
          whiteBlack[localPos] = 1'b0; // Set to black (0)
        end
      end
    end
  end

endmodule

// Top-level module for control
module image_processing_controller (
  input clk,
  input rst,
  input startRead,
  output reg readDone,
  output reg [10:0] greyScale_processed [0:imageSize-1],
  output reg [10:0] whiteBlack_processed [0:imageSize-1]
);
  parameter ROWS = 985;
  parameter COLS = 16;
  
  localparam imageSize = ROWS * COLS;

  // Instantiate the grey_scale_conversion module
  wire readDone_grey_process;
  wire [10:0] greyScale_wire [0:imageSize-1];

  grey_scale_conversion grey_scale_inst (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .readDone(readDone_grey_process),
    .greyScale(greyScale_wire)
  );

  // Instantiate the black_white_conversion module
  wire readDone_black_white_process;
  wire [10:0] whiteBlack_wire [0:imageSize-1];

  black_white_conversion black_white_inst (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .readDone(readDone_black_white_process),
    .whiteBlack(whiteBlack_wire)
  );

  // Connect readDone signals
  wire readDone_finished;
  assign readDone_finished = readDone_grey_process && readDone_black_white_process;

  // Output processed greyScale and whiteBlack signals
  assign greyScale_processed = greyScale_wire;
  assign whiteBlack_processed = whiteBlack_wire;

  // Finished the file conversion.
  assign readDone = readDone_finished;

endmodule
