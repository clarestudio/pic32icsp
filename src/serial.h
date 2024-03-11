void serial_set_fname(char const *);
void serial_baud(int);
int serial_open();
int serial_close();

int serial_save();
int serial_restore();
int serial_init();

int serial_rts_set(int);
int serial_cts_get(void);
int serial_dtr_set(int);
int serial_dsr_get(void);
int serial_cd_get(void);
int serial_ri_get(void);

int serial_putc(unsigned char);
int serial_getc();
int serial_putmem(void const *, size_t);
int serial_getmem(void *, size_t);
