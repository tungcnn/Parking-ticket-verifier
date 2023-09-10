module gray_scale_conversion_tb;

  // Inputs
  reg clk;
  reg rst;
  reg startRead;

  // Outputs
  wire readDone;
  wire [10:0] greyScale [0:15759];

  // Instantiate the module
  grey_scale_conversion uut (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .readDone(readDone),
    .greyScale(greyScale)
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

    // Print some elements from grayscale_avg_tb and grayscale_other_tb
    #10 for (int i = 0; i < 10; i = i + 1) begin
      $display("Greyscale[%0d] = %0d", i, greyScale[i]);
    end

    // End simulation
    $finish;
  end

endmodule
