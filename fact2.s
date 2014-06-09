        loadi  0 1     
        read   1      
        call   fact
        load   0 fact_result 
        write  0
        halt
fact:
        compri 1 1     
        jumpe  14     
        jumpl  14    
        call   mult
        load   0 mult_result 
        subi   1 1     
        call   fact
        load   0 fact_result  
        store  0 fact_result 
        return
mult:
        loadi  2 8     
        loadi  3 0    
        shr    1     
        store  2 mult_counter 
        getstat 2      
        andi   2 1
        compri 2 1
        jumpe  25     
        jump   26    
        add    3 0
        shl    0    
        load   2 mult_counter 
        subi   2 1     
        compri 2 0    
        jumpg  18
        store  3 mult_result 
        return
fact_result:
	noop           
mult_result:
        noop          
mult_counter:
        noop         
