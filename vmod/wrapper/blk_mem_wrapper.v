`timescale 1ns / 1ps

module blk_mem_wrapper #(
    parameter READ_LATENCY = 3,
    parameter ADDR_WIDTH = 12,
    parameter DATA_WIDTH = 32
    )(
    input wire clk_a,
    input wire arstz_aq,
    input wire [ADDR_WIDTH-1:0] addr,
    input wire en,
    input wire we,
    input wire [DATA_WIDTH-1:0] din,
    output wire [DATA_WIDTH-1:0] dout,
    output wire valid
    );
    
    reg en_to_blkmem;
    reg valid_to_ext;
    reg [1:0] wait_counter;
    reg [1:0] wait_counter_next;
    
    enum {IDLE , WAIT , READ} state_aq,state_next;
    // Sequential logics in FSM        
    always @(posedge clk_a, negedge arstz_aq )
    begin
        if(arstz_aq == 1'b0) state_aq <= IDLE;
        else state_aq <= state_next;
    end
    // Combinational logics in FSM
    always @(*) begin
        state_next = state_aq;
        case(state_aq)
        IDLE: begin
                if(en==1'b1 && we == 1'b0)begin
                    if(READ_LATENCY == 1) state_next = READ;
                    else state_next = WAIT;
                end
              end
        WAIT: begin
                if(wait_counter == READ_LATENCY-1) state_next = READ;
              end
        READ: begin
                state_next = IDLE;
              end
        endcase              
    end

    //always @(posedge clk_a,negedge arstz_aq)
    
    
    
    // Transition Condition for FSM
    always @(posedge clk_a, negedge arstz_aq)
        if(arstz_aq == 1'b0) wait_counter <=0;
        else wait_counter <= wait_counter_next;
    
    always @(*) begin
        wait_counter_next = wait_counter;
        case(state_aq)
            IDLE: wait_counter_next = 2'b1;
            WAIT: wait_counter_next = wait_counter + 1'b1;
            default: wait_counter_next = 0;
        endcase
    end
    
    // Output signals
    always @(*) begin
        en_to_blkmem = en;
        valid_to_ext = 0;
        case(state_aq)
        IDLE: begin
                en_to_blkmem = en;
                valid_to_ext = 0;               
              end
        WAIT: begin
                en_to_blkmem = 1;
                valid_to_ext = 0;
              end
        READ: begin
                en_to_blkmem = 0;
                valid_to_ext = 1;
              end
        endcase
    end
    // Connections
    assign valid = valid_to_ext;
    
    blk_mem_gen_0 your_instance_name (
      .clka(clk_a),    // input wire clka
      .ena(en_to_blkmem),      // input wire ena
      .wea(we),      // input wire [0 : 0] wea
      .addra(addr),  // input wire [9 : 0] addra
      .dina(din),    // input wire [31 : 0] dina
      .douta(dout)  // output wire [31 : 0] douta
    );
    
endmodule    

