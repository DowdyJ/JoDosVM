#pragma once
#include <stdio.h>
#include <cstdint>
#include <signal.h>
#include <Windows.h>
#include <conio.h>



class ExternalUtilities 
{
public:

	void Init();
	void CleanUp();

//private:
	static uint16_t check_key();
	void disable_input_buffering();
	//void restore_input_buffering();
	//void handle_interrupt(int signal);
	

};