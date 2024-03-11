void icsp_tap_idle();
void icsp_command(uint8_t);
uint8_t icsp_xfer_byte(uint8_t);
uint32_t icsp_xfer_dword(uint32_t);
uint32_t icsp_xfer_fastdata(uint32_t);
void icsp_xfer_insn(uint32_t);
uint32_t icsp_read_dword(uint32_t);

int icsp_enter(void);
int icsp_leave(void);
int icsp_init(void);
int icsp_check_idcode();
int icsp_erase_chip(void);
int icsp_enter_serial_exec(void);
void icsp_write_mem(uint32_t, uint8_t const *, size_t);
void icsp_read_mem(uint32_t, uint8_t *, size_t);
int icsp_flash_row(uint32_t addr, uint32_t srcaddr);
void icsp_rewind(void);
