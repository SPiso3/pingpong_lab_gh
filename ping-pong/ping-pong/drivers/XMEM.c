void XMEM_init(){
    MCUCR |= (1<<SRE);  //enable XMEM interface
    SFIOR |= (1<<XMM2); //mask-out 4msb A15-A12 setting XMM = 0b100
}

void XMEM_write(uint8_t data, uint16_t addr){
    volatile char *ext_mem = (char *) BASE ADDRESS
    ext_mem[addr] = data;
}

uint8_t XMEM_read(uint16_t addr){
    volatile char *ext_mem = (char *) BASE_ADDRESS;
    uint8_t ret_val = ext_mem[addr];
    return ret_val;
 }