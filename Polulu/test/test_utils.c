
#include "../uart.c"
#include "../utilities.c"


	
void main(){
	
	//vars
	char read_buffer[6];
	uint16_t read_value;
	int rtn_value;
	
	
	debug_frmwrk_init();
	uart_init();
	
	
	//test read sig
	rtn_value = request_sig(read_buffer);
	
	dbg_printf(read_buffer);
	dbg_printf("Return Value: %d \r\n", rtn_value);
	
	//test read battery voltage
	read_value = read_battery();
	dbg_printf("Battery Reading: %u \r\n", read_value);
	
	//test read trimpot
	read_value = read_trimpot();
	dbg_printf("Trimpot Reading: %u \r\n", read_value);
	
	
	
	
	/*  Monkey island (not quite working) -- need delay
	
	play_music("O6 T60 L4    e16 <b16 e16 g16    f#16 e16 d8    e. d8");
	play_music("d16 <c16 <b16 d16    <c8 <c8    <b    >e8 e8");
	play_music("O6 T60 L4    e8. g16    f#16 e16 d8    e. R16 f#16");
	play_music("O6 T60 L4    g8 g8    a    f#8. g16    f#16 e16 d16 f#16");
	play_music("O6 T60 L4    g8 g8    f#    e8. g16    f#16 e16 d16 f#16");
	play_music("O6 T60 L4    g8 g8    f#    e8. g16    f#16 e16 d8");
	play_music("O6 T60 L4    e8 e8    e    R8 e8    d16 <c16 <b16 d16");
	
	*/
	
	//test play music
	play_music("!O5 T70 L4 V8 MS"); 						//set Octave = 5, temp 70bpm, sig=4, staccrto mode 
	play_music("g4g4g4 e-8. b-16  g4 e-8 . b-16 g2  >d4 >d4 >d4 >e-8. b-16 g-4 e-8. b-16 g2");	//bar 1-4 imperial march
	
	//delay_music(70,4,4);  need a delay to play the a 2nd play string otherwise 1st will be overwritten
	
}
