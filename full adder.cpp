// File: full_adder_driver.h
#include<iostream.h>
#include<fstream.h>
#include "systemc.h"

SC_MODULE (driver)
{
sc_out<bool> d_a, d_b, d_cin;
bool t_a, t_b, t_cin;

ifstream infile;
void driver_prc();

SC_CTOR(driver)
	{
	SC_THREAD(driver_prc);
	infile.open("full_adder.in");
	if (! infile)
		{
		cerr<<"ERROR:Unable to open vector file"<<
		    "fa_with_ha.in!\n";
			sc_stop();
		}
	}
	//Close the file in the destructor
	~driver()
	{
	infile.close();
	}
};

//File: full_adder_driver.cpp
#include "full_adder_driver.h"

void driver :: driver_prc{}
{
sc_time apply_delay(5, SC_NS);
while(infile>>t_a>>t_b>>t_cin)
{
d_a.write(t_a);
d_b.write(t_b);
d_cin.write(t_cin);
wait(apply_delay);
}
}

//File:full_adder_monitor.h
#include<fstream.h>
#include"systemc.h"

SC_MODULE(monitor)
{
sc_in<bool>m_a, m_b, m_cin, m_sum, m_cout;

ofstream outfile;
void monitor_prc();

SC_CTOR(monitor)
{
SC_METHOD(monitor_prc);
sensitive<<m_a<<m_b<<m_cin<<m_sum<<m_cout;
outfile.open("full_adder.out");
}

//File: full_adder_monitor.cpp
#include"full_adder_monitor.h"

void monitor :: monitor_prc()
{
outfile<<"At time "<<sc_time_stamp()<<"::";
outfile<<"(a, b, carry_in):" ;
outfile<<m_a<<m_b<<m_cin;
outfile<<"(sum,carry_out):"<<m_sum<<m_cout<<'\n';
}

//File : full_adder_main.cpp
#include "full_adder_driver.h"
#include "full_adder_monitor.h"
#include "full_adder.h"

int sc_main(int argc, char* argv[])
{
sc_signal<bool>t_a, t_b, t_cin, t_sum, t_cout;

full_adder f1 ("FullAdderWithHalfAdder");
//Positional Association:
f1<<t_a<<t_b<<t_cin<<t_sum<<t_cout;

driver d1("Generatewaveforms");
d1<<t_a<<t_b<<t_cin;

monitor mo1("Monitorwaveforms");
mo1<<t_a<<t_b<<t_cin<<t_sum<<t_cout;

if(! mo1.outfile)
{
cerr<<""ERROR:Unable to open output file,"<<"fa_with_ha.out!\n";
return(-2);
}

sc_trace_file *tf=sc_create_vcd_trace_file("Full_adder");
sc_trace(tf, t_a, "A");
sc_trace(tf, t_b, "B");
sc_trace(tf, t_cin, "CarryIn");
sc_trace(tf, t_sum, "Sum");
sc_trace(tf, t_cout, "CarryOut");
sc_start(100, SC_NS);
sc_close_vcd_trace_file(tf);
return(0);
}

