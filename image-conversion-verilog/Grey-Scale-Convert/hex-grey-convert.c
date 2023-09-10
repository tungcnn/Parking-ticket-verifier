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
        for (localPos = 0; localPos < imageSize; localPos = localPos + 3) begin
          RED[assignColour] <= totalImageMemory[localPos];
          GREEN[assignColour] <= totalImageMemory[localPos + 1];
          BLUE[assignColour] <= totalImageMemory[localPos + 2];
          assignColour = assignColour + 1;
        end
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

  // Grayscale conversion using the average method
//   always @(posedge clk or posedge rst or posedge startRead) begin
//     for (localPos = 0; localPos < imageSize; localPos = localPos + 1) begin
//       greyScale[localPos] = (RED_rgb[localPos] + GREEN_rgb[localPos] + BLUE_rgb[localPos]) / 3;
//     end
//   end

  // Grayscale conversion using the luminosity method
  always @(posedge clk or posedge rst or posedge startRead) begin
    for (localPos = 0; localPos < imageSize; localPos = localPos + 1) begin
      greyScale[localPos] = (0.299 * RED_rgb[localPos]) + (0.587 * GREEN_rgb[localPos]) + (0.114 * BLUE_rgb[localPos]);
    end
  end

endmodule