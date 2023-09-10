module image_processing_controller_tb;

  // Local params
  parameter ROWS = 985;
  parameter COLS = 16;

  localparam imageSize = ROWS * COLS;
  
  // Inputs
  reg clk;
  reg rst;
  reg startRead;

  // Outputs
  wire readDone;
  wire [10:0] greyScale_processed [0:imageSize-1];
  wire [10:0] whiteBlack_processed [0:imageSize-1];

  // Instantiate the module
  image_processing_controller uut (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .readDone(readDone),
    .greyScale_processed(greyScale_processed),
    .whiteBlack_processed(whiteBlack_processed)
  );

  // Clock generation
  always begin
    #5 clk = ~clk;
  end

  // Test scenario
  initial begin
    clk = 0;
    rst = 0;
    startRead = 1;

    // Initialize signals
    #10 rst = 1;

    // Wait for reset to complete
    #10 rst = 0;

    // Start reading
    #10 startRead = 1;

    // Wait for readDone signal to go high
    #10 while (!readDone) begin
      #10;
    end

    // Print some elements from greyScale_processed and whiteBlack_processed
    #10 for (int i = 0; i < 10; i = i + 1) begin
      $display("GreyScale_processed[%0d] = %0d", i, greyScale_processed[i]);
      $display("WhiteBlack_processed[%0d] = %0d", i, whiteBlack_processed[i]);
    end

    // End simulation
    $finish;
  end

endmodule
