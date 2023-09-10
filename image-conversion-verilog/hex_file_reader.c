module hex_file_reader (
  input clk,
  input rst,
  input startRead,
  output reg readDone,
  output reg [9:0] RED [0:imageSize-1],
  output reg [9:0] GREEN [0:imageSize-1],
  output reg [9:0] BLUE [0:imageSize-1]
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

  always @(posedge clk or posedge rst) begin
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
          localPos <= localPos + 3;
          assignColour <= assignColour + 1;
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
  output reg [9:0] RED [0:imageSize-1],
  output reg [9:0] GREEN [0:imageSize-1],
  output reg [9:0] BLUE [0:imageSize-1]
);

  parameter ROWS = 985;
  parameter COLS = 16;
  parameter imageFile = "car.hex";

  localparam imageSize = ROWS * COLS;

  // Wire to connect to the readDone signal in hex_file_reader
  wire readDone_wire;
  wire [9:0] RED_wire [0:imageSize-1];
  wire [9:0] GREEN_wire [0:imageSize-1];
  wire [9:0] BLUE_wire [0:imageSize-1];

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
