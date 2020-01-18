/***************************************************************
 * Name:      PoluluNavMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Copyright:  ()
 * License:
 **************************************************************/

#include "PoluluNavMain.h"
#include <wx/msgdlg.h>
#include "wx/wx.h"
#include "wx/sizer.h"
#include <wx/bitmap.h>
#include <cstddef>

//(*InternalHeaders(PoluluNavFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)
using namespace std;
//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}


//(*IdInit(PoluluNavFrame)
const long PoluluNavFrame::ID_BUTTON1 = wxNewId();
const long PoluluNavFrame::ID_PANEL2 = wxNewId();
const long PoluluNavFrame::ID_BUTTON2 = wxNewId();
const long PoluluNavFrame::ID_PANEL3 = wxNewId();
const long PoluluNavFrame::ID_BUTTON3 = wxNewId();
const long PoluluNavFrame::ID_SLIDER1 = wxNewId();
const long PoluluNavFrame::ID_BUTTON4 = wxNewId();
const long PoluluNavFrame::ID_BUTTON5 = wxNewId();
const long PoluluNavFrame::ID_BUTTON6 = wxNewId();
const long PoluluNavFrame::ID_BUTTON7 = wxNewId();
const long PoluluNavFrame::ID_BUTTON8 = wxNewId();
const long PoluluNavFrame::ID_BUTTON9 = wxNewId();
const long PoluluNavFrame::ID_SPINCTRL1 = wxNewId();
const long PoluluNavFrame::ID_SPINCTRL2 = wxNewId();
const long PoluluNavFrame::ID_STATICTEXT1 = wxNewId();
const long PoluluNavFrame::ID_STATICTEXT2 = wxNewId();
const long PoluluNavFrame::ID_STATICTEXT3 = wxNewId();
const long PoluluNavFrame::ID_STATICTEXT4 = wxNewId();
const long PoluluNavFrame::ID_STATICTEXT5 = wxNewId();
const long PoluluNavFrame::ID_STATICTEXT6 = wxNewId();
const long PoluluNavFrame::ID_CHECKBOX1 = wxNewId();
const long PoluluNavFrame::ID_CHECKBOX2 = wxNewId();
const long PoluluNavFrame::ID_CHECKBOX3 = wxNewId();
const long PoluluNavFrame::ID_CHECKBOX4 = wxNewId();
const long PoluluNavFrame::ID_CHECKBOX5 = wxNewId();
const long PoluluNavFrame::ID_SLIDER2 = wxNewId();
const long PoluluNavFrame::ID_STATICTEXT7 = wxNewId();
const long PoluluNavFrame::ID_SPINCTRL3 = wxNewId();
const long PoluluNavFrame::ID_PANEL1 = wxNewId();
const long PoluluNavFrame::ID_TIMER1 = wxNewId();
//*)

double CurrentX = 1024;
double CurrentY = 1024;
int WindowX = 1024; int WindowY = 1024;
int Angle = 0, Angle3d = 0;;
wxBitmap* ImgBuffer;
wxMemoryDC* dcImgBuffer;
RecordedRobotData Map(4096);
Direction RobotDirection = D_STOP;
int tempcount = 0;
int printline = 0;
FILE *fp;
int PlotMax = 0;

BEGIN_EVENT_TABLE(PoluluNavFrame,wxFrame)
    //(*EventTable(PoluluNavFrame)
    //*)
END_EVENT_TABLE()

PoluluNavFrame::PoluluNavFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PoluluNavFrame)
    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(950,640));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(128,144), wxSize(1072,600), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Read Sensor Data"), wxPoint(536,16), wxSize(120,27), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    MainPanel = new wxPanel(Panel1, ID_PANEL2, wxPoint(16,16), wxSize(512,512), wxSIMPLE_BORDER, _T("ID_PANEL2"));
    Button2 = new wxButton(Panel1, ID_BUTTON2, _("Plot Data"), wxPoint(536,56), wxSize(120,27), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Panel2 = new wxPanel(Panel1, ID_PANEL3, wxPoint(668,16), wxSize(264,496), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    Button3 = new wxButton(Panel1, ID_BUTTON3, _("PrintAll"), wxPoint(536,504), wxSize(120,27), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    Slider1 = new wxSlider(Panel1, ID_SLIDER1, -1, -1, 100, wxPoint(16,568), wxSize(504,24), 0, wxDefaultValidator, _T("ID_SLIDER1"));
    Button4 = new wxButton(Panel1, ID_BUTTON4, _("↑"), wxPoint(584,544), wxSize(27,27), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    Button5 = new wxButton(Panel1, ID_BUTTON5, _("→"), wxPoint(608,568), wxSize(27,27), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    Button6 = new wxButton(Panel1, ID_BUTTON6, _("↓"), wxPoint(584,592), wxSize(27,27), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    Button7 = new wxButton(Panel1, ID_BUTTON7, _("←"), wxPoint(560,568), wxSize(27,27), 0, wxDefaultValidator, _T("ID_BUTTON7"));
    Button8 = new wxButton(Panel1, ID_BUTTON8, _("Paint Buffer"), wxPoint(536,96), wxSize(120,27), 0, wxDefaultValidator, _T("ID_BUTTON8"));
    Button9 = new wxButton(Panel1, ID_BUTTON9, _("3D"), wxPoint(680,544), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    Button9->Hide();
    SpinCtrl1 = new wxSpinCtrl(Panel1, ID_SPINCTRL1, _T("30"), wxPoint(536,160), wxDefaultSize, 0, 0, 300, 30, _T("ID_SPINCTRL1"));
    SpinCtrl1->SetValue(_T("30"));
    SpinCtrl2 = new wxSpinCtrl(Panel1, ID_SPINCTRL2, _T("250"), wxPoint(536,208), wxDefaultSize, 0, 0, 300, 250, _T("ID_SPINCTRL2"));
    SpinCtrl2->SetValue(_T("250"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Sensor Min Range"), wxPoint(536,144), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Sensor Max Range"), wxPoint(536,192), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Angle:"), wxPoint(576,248), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Label"), wxPoint(624,248), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("Points to Plot:"), wxPoint(536,264), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, _("Label"), wxPoint(624,264), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    CheckBox1 = new wxCheckBox(Panel1, ID_CHECKBOX1, _("3D"), wxPoint(536,392), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox1->SetValue(false);
    CheckBox2 = new wxCheckBox(Panel1, ID_CHECKBOX2, _("Left Sensor"), wxPoint(536,344), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBox2->SetValue(true);
    CheckBox3 = new wxCheckBox(Panel1, ID_CHECKBOX3, _("Front Sensor"), wxPoint(536,296), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBox3->SetValue(true);
    CheckBox4 = new wxCheckBox(Panel1, ID_CHECKBOX4, _("Right Sensor"), wxPoint(536,368), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBox4->SetValue(true);
    CheckBox5 = new wxCheckBox(Panel1, ID_CHECKBOX5, _("Back Sensor"), wxPoint(536,320), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    CheckBox5->SetValue(true);
    Slider2 = new wxSlider(Panel1, ID_SLIDER2, 0, 0, 359, wxPoint(536,416), wxSize(120,24), 0, wxDefaultValidator, _T("ID_SLIDER2"));
    StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, _("Image Scale"), wxPoint(536,456), wxSize(-1,-1), 0, _T("ID_STATICTEXT7"));
    SpinCtrl3 = new wxSpinCtrl(Panel1, ID_SPINCTRL3, _T("4"), wxPoint(536,472), wxDefaultSize, 0, 1, 100, 4, _T("ID_SPINCTRL3"));
    SpinCtrl3->SetValue(_T("4"));
    Timer1.SetOwner(this, ID_TIMER1);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton1Click);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton2Click);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton3Click);
    Connect(ID_SLIDER1,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&PoluluNavFrame::OnSlider1CmdScrollChanged);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton4Click);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton5Click);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton6Click);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton7Click);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton8Click1);
    Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PoluluNavFrame::OnButton9Click);
    Connect(ID_SLIDER2,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&PoluluNavFrame::OnSlider2CmdScrollChanged);
    Connect(ID_SPINCTRL3,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&PoluluNavFrame::OnSpinCtrl3Change);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&PoluluNavFrame::OnTimer1Trigger2);
    //*)
    SetClientSize(wxSize(670,640)); //Remove for debug
    ImgBuffer = new wxBitmap(2048,2048);
    dcImgBuffer = new wxMemoryDC;
    dcImgBuffer->SelectObject(*ImgBuffer);
    dcImgBuffer->SetBrush(wxBrush(wxColour(255,255,255)));
    dcImgBuffer->DrawRectangle(-1, -1, 2049, 2049);
    ReadSensorData();
}

PoluluNavFrame::~PoluluNavFrame()
{
    //(*Destroy(PoluluNavFrame)
    //*)
}

void PoluluNavFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void PoluluNavFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}


void PoluluNavFrame::OnButton1Click(wxCommandEvent& event)
{
    ReadSensorData();
    /*int* temp = Map.Get(Map.Count()-10+tempcount); tempcount++;
    stringstream ss;
    ss << temp[0] << " " << temp[1] << " " << temp[2] << " " << temp[3] << " " << temp[4];
    PrintStr(ss.str());*/


}

void PoluluNavFrame::OnButton2Click(wxCommandEvent& event)
{
    PlotMax = -1;
    PlotAllSensorData();
    Angle = 0;
    /*dcImgBuffer->DrawRectangle(-1, -1, 2049, 2049);
    int* temp;
        temp = Map.Get(Map.Count()-20);
        if (temp == 0) return;
        if (temp[0] == IR)
        {
            PlotSensorReading(FRONT, temp[1]);
            PlotSensorReading(LEFT, temp[2]);
            PlotSensorReading(RIGHT, temp[3]);
            PlotSensorReading(BACK, temp[4]);
        }
    stringstream ss;
    ss << temp[0] << " " << temp[1] << " " << temp[2] << " " << temp[3] << " " << temp[4];
    PrintStr(ss.str());*/


    //PaintBuffer();
}

void PoluluNavFrame::OnButton3Click(wxCommandEvent& event)
{
    ReadSensorData();
    PlotMax = -1;
    PlotAllSensorData();
    PaintBuffer();
}

void PoluluNavFrame::PlotSensorReading(Sensor s, int value)
{
    //int DisplayMod = 5;
    //int minRange = 16;
    //int maxRange = 170 / 4;
    int DisplayMod = 5;
    int minRange = SpinCtrl1->GetValue();
    int maxRange = SpinCtrl2->GetValue();
    if ((Angle < -360) || (Angle > 360)) Angle %= 360;

    value /= DisplayMod;
    if (!((value > minRange) && (value < maxRange))) return;
    switch (s)
    {
        case FRONT:
            //if ((value > minRange) && (value < maxRange))
            dcImgBuffer->DrawRectangle(CurrentX-4 + sin(Angle * PI / 180) * value, CurrentY-4 -cos(Angle * PI / 180)* value, 2, 2 );
        break;

        case BACK:
            //if ((value > minRange) && (value < maxRange))
            dcImgBuffer->DrawRectangle(CurrentX-4 + sin(Angle * PI / 180) * value, CurrentY-4 +cos(Angle * PI / 180)* value, 2, 2 );
        break;

        case LEFT:
            dcImgBuffer->DrawRectangle(CurrentX-4 + sin((Angle-90) * PI / 180) * value, CurrentY-4 -cos((Angle-90) * PI / 180)* value, 2, 2 );
        break;

        case RIGHT:
            dcImgBuffer->DrawRectangle(CurrentX-4 + sin((Angle+90) * PI / 180) * value, CurrentY-4 -cos((Angle+90) * PI / 180)* value, 2, 2 );
        break;
    }
}

 void PoluluNavFrame::ReadSensorData()
 {
    Angle = 0; // remove for funny results
    Map.Clear();
    ifstream myReadFile;
    myReadFile.open("/home/nathan/screenlog.0");//ofstream program3data;program3data.open("/home/nathan/program3data.txt");
    Map.Clear();
    string currentline;
    if (myReadFile.is_open())
    {
        while (!myReadFile.eof())
        {
            myReadFile >> currentline;
            if (currentline[0] == 'M') //Motor command
            {
                int temp[Cols];
                temp[0] = MOTOR;
                myReadFile >> currentline;
                temp[1] = atoi(currentline.c_str());
                myReadFile >> currentline;
                temp[2] = atoi(currentline.c_str());
                Map.Add(temp);
            }
            else if (currentline[0] == 'I') //Sensor command
            {
                int temp[Cols];
                temp[0] = IR; //program3data << IR + " ";
                myReadFile >> currentline; //program3data << currentline + " ";
                temp[1] = atoi(currentline.c_str()); //Front
                myReadFile >> currentline; //program3data << currentline + " ";
                temp[2] = atoi(currentline.c_str()); //Left
                myReadFile >> currentline; //program3data << currentline + " ";
                temp[3] = atoi(currentline.c_str()); //Right
                myReadFile >> currentline; //program3data << currentline + "\n";
                temp[4] = atoi(currentline.c_str()); //Back
                Map.Add(temp);
            }
        }
    }
    myReadFile.close();
    //program3data.close();
    Slider1->SetMax(Map.Count()); //slide to number of points
 }


void PoluluNavFrame::PaintBuffer()
{
    wxPaintDC dcMainPanel(MainPanel);
    int x; int y;
    MainPanel->GetSize(&x,&y);
    x = x / 2;  y = y / 2;
    dcImgBuffer->DrawCircle(CurrentX-4,CurrentY-4,8);
    dcImgBuffer->DrawLine(CurrentX-4,CurrentY-4,CurrentX-4+sin(Angle * PI / 180)*8,CurrentY-4-cos(Angle * PI / 180)*8); //Draw robot direction line
    dcMainPanel.Blit(wxPoint(0,0),dcImgBuffer->GetSize(), &(*dcImgBuffer), wxPoint(WindowX-x, WindowY-y), wxCOPY, true); //Draw
    //dcMainPanel.DrawCircle(x-4,y-4,8);
    //dcMainPanel.DrawLine(CurrentX-4,CurrentY-4,CurrentX-4+sin(Angle * PI / 180)*8,CurrentY-4-cos(Angle * PI / 180)*8); //Draw robot direction line
}

void PoluluNavFrame::PlotAllSensorData()
{
    RobotDirection = D_STOP;
    CurrentX = 1024; CurrentY = 1024; Angle = 0;
    dcImgBuffer->DrawRectangle(-1, -1, 2049, 2049);
    if (PlotMax == -1) PlotMax = Map.Count();
    int* temp;
    for (int i = 0; i < PlotMax; i++)
    {
        temp = Map.Get(i);
        if (temp[0] == IR)
        {
            if (CheckBox1->GetValue()) Angle3d = Slider2->GetValue();
            else Angle3d = 0;

            if (CheckBox3->GetValue()) PlotSensorReading1(FRONT, temp[1]);

            if (CheckBox2->GetValue()) PlotSensorReading1(LEFT, temp[2]);

            if (CheckBox4->GetValue()) PlotSensorReading1(RIGHT, temp[3]);

            if (CheckBox5->GetValue()) PlotSensorReading1(BACK, temp[4]);

            //CurrentY+=2;
            if (RobotDirection == D_FORWARD)
            {
                CurrentX += sin(Angle * PI / 180);
                CurrentY -= cos(Angle * PI / 180);
            }
            else if (RobotDirection == D_BACK)
            {
                CurrentX -= sin(Angle * PI / 180);
                CurrentY += cos(Angle * PI / 180);
            }
        }
        else if (temp[0] == MOTOR)
        {
            if ((temp[1] == temp[2]) && (temp[1] > 0)) RobotDirection = D_FORWARD;
            else if (temp[1] > temp[2]) RobotDirection = D_RIGHT;
            else if (temp[1] < temp[2]) RobotDirection = D_LEFT;
            else if ((temp[1] == temp[2]) && (temp[1] < 0)) RobotDirection = D_BACK;
            else if ((temp[1] == 0) && (temp[2] == 0)) RobotDirection = D_STOP;
            //RobotDirection
        }
        if (RobotDirection == D_RIGHT) Angle += 360 / 31;
        if (RobotDirection == D_LEFT) Angle -= 360 / 31;

        dcImgBuffer->SetPen( wxPen( wxColor(255,0,0), 1 ) );
        dcImgBuffer->DrawRectangle(CurrentX-4, CurrentY-4, 2, 2 );
        dcImgBuffer->SetPen( wxPen( wxColor(0,0,0), 1 ) );
    }
    //dcImgBuffer->DrawLine(1024,0,2048,2048); dcImgBuffer->DrawLine(1024,1024,0,4096);
    //Angle+=10;
    //PaintBuffer();
    wxString theStr;
    theStr << Angle;
    StaticText4->SetLabel(theStr);

}

void PoluluNavFrame::PrintStr(string val)
{
    wxPaintDC dcMainPanel(Panel2);
    wxString mystring(val.c_str(), wxConvUTF8);
    //dcMainPanel.Clear();
    dcMainPanel.DrawText(mystring,0,10 * printline);
    printline++;
}

void PoluluNavFrame::PrintInt(int val)
{
    wxPaintDC dcMainPanel(Panel2);
    wxString theStr;
    theStr << val;
    dcMainPanel.DrawText(theStr,0,10 * printline);
    printline++;
}


void PoluluNavFrame::OnTimer1Trigger2(wxTimerEvent& event)
{
    ReadSensorData();
    //PlotAllSensorData();
    dcImgBuffer->DrawRectangle(-1, -1, 2049, 2049);
    int* temp;
        temp = Map.Get(Map.Count()-2);
        if (temp[0] == IR)
        {
            PlotSensorReading(FRONT, temp[1]);
            PlotSensorReading(LEFT, temp[2]);
            PlotSensorReading(RIGHT, temp[3]);
            PlotSensorReading(BACK, temp[4]);
        }
    stringstream ss;
    //ss << temp[0] << " " << temp[1] << " " << temp[2] << " " << temp[3] << " " << temp[4];
    //PrintStr(ss.str());


    PaintBuffer();
}



void PoluluNavFrame::OnSlider1CmdScrollChanged(wxScrollEvent& event)
{
    PlotMax = Slider1->GetValue();
    PlotAllSensorData();
    //CurrentX = 1024;
    //CurrentY = 1024;
    wxString theStr;
    theStr << PlotMax;
    StaticText6->SetLabel(theStr);
    PaintBuffer();
}

void PoluluNavFrame::OnButton4Click(wxCommandEvent& event)
{
    WindowY+=10;
    PaintBuffer();
}

void PoluluNavFrame::OnButton8Click1(wxCommandEvent& event)
{
    PaintBuffer();
}

void PoluluNavFrame::OnButton7Click(wxCommandEvent& event)
{
    WindowX+=10;
    PaintBuffer();
}

void PoluluNavFrame::OnButton5Click(wxCommandEvent& event)
{
    WindowX+=-10;
    PaintBuffer();
}

void PoluluNavFrame::OnButton6Click(wxCommandEvent& event)
{
    WindowY+=-10;
    PaintBuffer();
}

void PoluluNavFrame::OnButton9Click(wxCommandEvent& event)
{
    Angle3d+=10;
    RobotDirection = D_STOP;
    CurrentX = 1024; CurrentY = 1024; Angle = 0;
    dcImgBuffer->DrawRectangle(-1, -1, 2049, 2049);
    if (PlotMax == -1) PlotMax = Map.Count();
    int* temp;
    for (int i = 0; i < PlotMax; i++)
    {
        temp = Map.Get(i);
        if (temp[0] == IR)
        {
            PlotSensorReading1(FRONT, temp[1]);
            PlotSensorReading1(LEFT, temp[2]);
            PlotSensorReading1(RIGHT, temp[3]);
            PlotSensorReading1(BACK, temp[4]);
            //CurrentY+=2;
            if (RobotDirection == D_FORWARD)
            {
                CurrentX += sin(Angle * PI / 180);
                CurrentY -= cos(Angle * PI / 180);
            }
            else if (RobotDirection == D_BACK)
            {
                CurrentX -= sin(Angle * PI / 180);
                CurrentY += cos(Angle * PI / 180);
            }
        }
        else if (temp[0] == MOTOR)
        {
            if ((temp[1] == temp[2]) && (temp[1] > 0)) RobotDirection = D_FORWARD;
            else if (temp[1] > temp[2]) RobotDirection = D_RIGHT;
            else if (temp[1] < temp[2]) RobotDirection = D_LEFT;
            else if ((temp[1] == temp[2]) && (temp[1] < 0)) RobotDirection = D_BACK;
            else if ((temp[1] == 0) && (temp[2] == 0)) RobotDirection = D_STOP;
            //RobotDirection
        }
        if (RobotDirection == D_RIGHT) Angle += 360 / 31;
        if (RobotDirection == D_LEFT) Angle -= 360 / 31;

        dcImgBuffer->SetPen( wxPen( wxColor(255,0,0), 1 ) );
        dcImgBuffer->DrawRectangle(CurrentX-4, CurrentY-4, 2, 2 );
        dcImgBuffer->SetPen( wxPen( wxColor(0,0,0), 1 ) );
    }
    PaintBuffer();
}





void PoluluNavFrame::PlotSensorReading1(Sensor s, int value)
{
    int DisplayMod = 5;
    int minRange = SpinCtrl1->GetValue();
    int maxRange = SpinCtrl2->GetValue();
    if ((Angle < -360) || (Angle > 360)) Angle %= 360;


    if (!((value > minRange) && (value < maxRange))) return;
    value /= SpinCtrl3->GetValue();
    switch (s)
    {
        case FRONT:
            //if ((value > minRange) && (value < maxRange))
            dcImgBuffer->DrawRectangle(CurrentX-4 + sin((Angle+Angle3d) * PI / 180) * value, CurrentY-4 -cos((Angle+Angle3d) * PI / 180)* value, 2, 2 );
        break;

        case BACK:
            //if ((value > minRange) && (value < maxRange))
            dcImgBuffer->DrawRectangle(CurrentX-4 + sin((Angle+Angle3d) * PI / 180) * value, CurrentY-4 +cos((Angle+Angle3d) * PI / 180)* value, 2, 2 );
        break;

        case LEFT:
            dcImgBuffer->DrawRectangle(CurrentX-4 + sin(((Angle+Angle3d)-90) * PI / 180) * value, CurrentY-4 -cos(((Angle+Angle3d)-90) * PI / 180)* value, 2, 2 );
        break;

        case RIGHT:
            dcImgBuffer->DrawRectangle(CurrentX-4 + sin(((Angle+Angle3d)+90) * PI / 180) * value, CurrentY-4 -cos(((Angle+Angle3d)+90) * PI / 180)* value, 2, 2 );
        break;
    }
}

void PoluluNavFrame::OnSlider2CmdScrollChanged(wxScrollEvent& event)
{
    Angle3d = Slider2->GetValue();
    PlotAllSensorData();
    PaintBuffer();
}

void PoluluNavFrame::OnSpinCtrl3Change(wxSpinEvent& event)
{
    PlotAllSensorData();
    PaintBuffer();
}
