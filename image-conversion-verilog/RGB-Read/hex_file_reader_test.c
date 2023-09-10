module hex_file_reader_tb;

  // Inputs
  reg clk;
  reg rst;
  reg startRead;

  // Outputs
  wire readDone;
  wire [9:0] RED_tb [0:15759];
  wire [9:0] GREEN_tb [0:15759];
  wire [9:0] BLUE_tb [0:15759];

  // Instantiate the module
  rgb_data_reader uut (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .readDone(readDone),
    .RED(RED_tb),
    .GREEN(GREEN_tb),
    .BLUE(BLUE_tb)
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

    // Print some elements from RED, GREEN, and BLUE
    #10 for (int i = 0; i < 10; i = i + 1) begin
      $display("RGB[%0d] = (%h, %h, %h)", i, RED_tb[i], GREEN_tb[i], BLUE_tb[i]);
    end

    // End simulation
    $finish;
  end

endmodule
