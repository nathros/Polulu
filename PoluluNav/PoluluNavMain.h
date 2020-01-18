/***************************************************************
 * Name:      PoluluNavMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef POLULUNAVMAIN_H
#define POLULUNAVMAIN_H

//(*Headers(PoluluNavFrame)
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/timer.h>
//*)
#include <string>
#include <iostream>
#include <fstream>

#include <cstdlib>
#include <math.h>
#include <sstream>
#define PI 3.14159265
typedef enum{FRONT, LEFT, RIGHT, BACK} Sensor;
typedef enum{D_FORWARD, D_LEFT, D_RIGHT, D_BACK, D_STOP} Direction;
typedef enum{MOTOR = 0, IR = 1, LINE, OTHER} Commands;


const int Cols = 10;

class RecordedRobotData
 {
    private:
        int** Data;
        int Max;
        int count;

    public:
        RecordedRobotData(int startsize)
        {
            Data = new int*[startsize];
            for (int i = 0; i < startsize; i++) Data[i] = new int[Cols];
            Max = startsize;                              //[0] = Commands
            count = 0;
        }

        ~RecordedRobotData()
        {

        }

        void Add(int newdata[])
        {
            for (int i = 0; i < Cols; i++) Data[count][i] = newdata[i];
            count++;
            if (count == Max)
            {
                //Make copy and expand
            }
        }

        int* Get(int row)
        {
            return Data[row];
        }

        int Count() {return count;}

        void Clear() {count = 0;} //temp
 };


class PoluluNavFrame: public wxFrame
{
    public:

        PoluluNavFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PoluluNavFrame();

    private:

        //(*Handlers(PoluluNavFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnTimer1Trigger1(wxTimerEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnTimer1Trigger2(wxTimerEvent& event);
        void OnPanel1Paint(wxPaintEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        void OnSlider1CmdScrollChanged(wxScrollEvent& event);
        void OnButton4Click(wxCommandEvent& event);
        void OnButton8Click(wxCommandEvent& event);
        void OnButton8Click1(wxCommandEvent& event);
        void OnButton7Click(wxCommandEvent& event);
        void OnButton5Click(wxCommandEvent& event);
        void OnButton6Click(wxCommandEvent& event);
        void OnButton9Click(wxCommandEvent& event);
        void OnSlider2CmdScrollChanged(wxScrollEvent& event);
        void OnSpinCtrl3Change(wxSpinEvent& event);
        //*)
        void PlotSensorReading(Sensor s, int value);
        void PlotSensorReading1(Sensor s, int value);
        void PlotAllSensorData();
        void ReadSensorData();
        void PaintBuffer();

        void PrintStr(std::string val);
        void PrintInt(int val);

        //(*Identifiers(PoluluNavFrame)
        static const long ID_BUTTON1;
        static const long ID_PANEL2;
        static const long ID_BUTTON2;
        static const long ID_PANEL3;
        static const long ID_BUTTON3;
        static const long ID_SLIDER1;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_BUTTON7;
        static const long ID_BUTTON8;
        static const long ID_BUTTON9;
        static const long ID_SPINCTRL1;
        static const long ID_SPINCTRL2;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_STATICTEXT6;
        static const long ID_CHECKBOX1;
        static const long ID_CHECKBOX2;
        static const long ID_CHECKBOX3;
        static const long ID_CHECKBOX4;
        static const long ID_CHECKBOX5;
        static const long ID_SLIDER2;
        static const long ID_STATICTEXT7;
        static const long ID_SPINCTRL3;
        static const long ID_PANEL1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(PoluluNavFrame)
        wxSlider* Slider1;
        wxButton* Button4;
        wxSlider* Slider2;
        wxSpinCtrl* SpinCtrl1;
        wxStaticText* StaticText2;
        wxButton* Button1;
        wxStaticText* StaticText6;
        wxCheckBox* CheckBox3;
        wxCheckBox* CheckBox2;
        wxPanel* Panel1;
        wxStaticText* StaticText1;
        wxStaticText* StaticText3;
        wxButton* Button2;
        wxCheckBox* CheckBox1;
        wxButton* Button6;
        wxButton* Button5;
        wxButton* Button3;
        wxSpinCtrl* SpinCtrl3;
        wxStaticText* StaticText5;
        wxStaticText* StaticText7;
        wxButton* Button7;
        wxButton* Button9;
        wxCheckBox* CheckBox4;
        wxSpinCtrl* SpinCtrl2;
        wxPanel* MainPanel;
        wxPanel* Panel2;
        wxCheckBox* CheckBox5;
        wxStaticText* StaticText4;
        wxButton* Button8;
        wxTimer Timer1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // POLULUNAVMAIN_H
