module hex_file_reader (
  input clk,
  input rst,
  input startRead,  
  output read_done  
);


  parameter ROWS = 102;
  parameter COLS = 51;
  parameter imageSize = 5202;
  parameter imageFile = "car.hex";
  
  reg [32:0] totalImageMemory [0 : imageSize-1];
  reg readDoneReg;
  
  integer loadedBMP   [0 : imageSize - 1];   
  integer localPos = 0;
  
  integer org_R  [0 : (imageSize/3) - 1];  
  integer org_G  [0 : (imageSize/3)- 1]; 
  integer org_B  [0 : (imageSize/3) - 1]; 

initial begin
$readmemh(imageFile, totalImageMemory, 0, imageSize-1);
end
  
  always @(posedge clk or posedge rst) begin
    if (rst) begin
      // Reset logic here
      readDoneReg <= 1'b0;
    end else begin
      // Read logic
      if (startRead) begin
        for (int i = 0; i < imageSize; i = i + 1) begin
          loadedBMP[i] = totalImageMemory[i];
        end
       
        for (int i = 0; i < ROWS; i = i + 1) begin
          for (int j = 0; j < COLS; j = j + 1) begin
            localPos = i * COLS + j; 
        
            // Save Red, Green, and Blue components
            org_R[localPos] = temp_BMP[localPos * 3 + 0];
            org_G[localPos] = temp_BMP[localPos * 3 + 1]; 
            org_B[localPos] = temp_BMP[localPos * 3 + 2]; 
          end
        end

      // Set read_done
        readDoneReg <= 1'b1;
      end else begin
        // Reset read_done when not reading
        readDoneReg <= 1'b0;
      end
    end
  end
  
    assign read_done = readDoneReg;
endmodule

// Testbench for verilog module ....................................

module hex_file_reader_tb;

  // Inputs
  reg clk;
  reg rst;
  reg startRead;
  
  // Outputs
  wire read_done;

  // Instantiate the module
  hex_file_reader uut (
    .clk(clk),
    .rst(rst),
    .startRead(startRead),
    .read_done(read_done)
  );

  // Clock generation
  always begin
    #5 clk = ~clk;
  end

  // Test scenario
  initial begin
    clk = 0;
    rst = 0;
    startRead = 0;
    
    // Reset the module
    rst = 1;
    #10 rst = 0;
    
    // Start reading
    startRead = 1;
    #100;
    startRead = 0;
    
    // Wait for the read to complete 
    #1000;
    
    // Print the read data and read_done signal
    $display("read_done = %b", read_done);
    
    // End simulation
    $finish;
  end

endmodule
