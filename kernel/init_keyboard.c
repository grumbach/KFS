void kb_init(void)

{

	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/

	write_port(0x21 , 0xFD);

}
