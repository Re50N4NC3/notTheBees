//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define f_amount 15
#define b_amount 100

#define TOP 20
#define BOT 600
#define LEF 20
#define RIG 780

#define v_max 5

TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
// functions
// return sign of the value
int sign(float n){
        if (n > 0)  { return  1; }
        if (n < 0)  { return -1; }
        else        { return  0; }
}

// calculate distance between the points
float dist(float x1, float y1, float x2, float y2){
        float val;
        val = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        return val;
}

// clamp value at other value
float clamp(float n, float cap){
        if ( n > cap){ return cap;}
        else{ return n;}
}

//---------------------------------------------------------------------------
class flower{
        public:
                float xx, yy, honey;

                TShape *shp;

                flower(void);


};

flower *lawn[f_amount];  // flowers pointer
int hive_size = 0;

// flower class
flower::flower(){
        xx=750-random(200);
        yy=650-random(600); // pozycja kwiatka

        shp=new TShape(Form1);
        shp -> Parent = Form1;
        shp -> Left = xx;
        shp -> Top = yy;
        shp -> Width = 10;
        shp -> Height = 10;
        shp -> Brush -> Color = clRed;
        shp -> Shape = stCircle;
}
//--------------------------------------------------------------------------
// hive class
class bee_hive{
        public:
                float xx, yy, honey_hive;

                TShape *shp;

                void add_bee();

                bee_hive(void);
};

bee_hive *home;  // pointer of the beehive

// beehive scope
bee_hive::bee_hive(){
        xx = 50 + random(60);
        yy = 280 + random(40); // pozycja ula
        honey_hive = 0;

        shp=new TShape(Form1);
        shp-> Parent = Form1;
        shp-> Left = xx;
        shp-> Top = yy;
        shp-> Width = 50;
        shp-> Height = 50;
        shp-> Brush -> Color = clOlive;
        shp-> Shape = stRectangle;
}

//--------------------------------------------------------------------------
// bee creation
class bee{
        public:
                float xx, yy, vx, vy, acc;
                int carry, target;

                TShape *shp;

                void move();

                bee(void);
};

bee *hive[b_amount];  // bees pointer


// bee scope
bee::bee(){
        xx = 50 + random(100);
        yy = 250 + random(100);
        vx = 2 - random(4);
        vy = 2 - random(4);
        acc = 0.3;
        carry = 0;
        target = random(f_amount - 1);

        shp = new TShape(Form1);
        shp -> Parent = Form1;
        shp -> Left = xx;
        shp -> Top = yy;
        shp -> Width = 4;
        shp -> Height = 4;
        shp -> Brush -> Color = clYellow;
        shp -> Shape = stCircle;
}

// bee movement script
void bee::move(){
	// check if bee is carrying the honey
        if (carry == 0){  // go to the flower
                vx += acc * sign(lawn[target] -> xx - xx);
                vy += acc * sign(lawn[target] -> yy - yy);
        }
        if (carry == 1){  // go to the beehive
                vx += acc * sign(home -> xx - xx) * (abs(xx - home -> xx) / 200) + 1 * sign(home -> xx - xx);
                vy += acc * sign(home -> yy - yy) * (abs(yy - home -> yy) / 200) + 1 * sign(home -> yy - yy);
        }

	// clamp the speed value
        if (abs(vx) > v_max){
                vx = sign(vx) * clamp(abs(vx), v_max);
        }
        if (abs(vy) > v_max){
                vy = sign(vy) * clamp(abs(vy), v_max);
        }


        xx += vx;
        yy += vy;  // change position values

        shp -> Left = xx;
        shp -> Top = yy;  // change position of the graphical representation

        // boundary collision check
        if (xx >= RIG) {vx = -vx; xx = RIG - 1; }
        if (xx <= LEF) {vx = -vx; xx = LEF + 1; }
        if (yy <= TOP) {vy = -vy; yy = TOP + 1; }
        if (yy >= BOT) {vy = -vy; yy = BOT - 1; }

        // detect if is near the flower
        if (carry == 0 && dist(xx, yy, lawn[target] -> xx, lawn[target] -> yy) < 10){
                carry = 1;
        }

        // detect if is near beehive
        if (carry == 1 && dist(xx, yy, home -> xx, home -> yy) < 20){
                carry = 0;
                home -> honey_hive += 10;
        }
}

// add bees by beehive
void bee_hive::add_bee(){

	// check if has enough honey to create new bee
        if (hive_size < b_amount && honey_hive >= 100 + hive_size * 3){
                hive[hive_size] = new bee;  // add the bee to the hive
                honey_hive -= 100 + hive_size * 3;  // decrease honey amount
                hive_size += 1;  // incrase bees counter
        }
}

//----------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender){
        int i;

	// create flowers
        for (i = 0; i < f_amount; i++){
                lawn[i] = new flower;
        }

        Button1 -> Enabled = 0;  // disable flowers button
        Button2 -> Enabled = 1;  // enable flowers button
}

//----------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender){
        int i;
	
	// create new bees
        for (i=0; i < b_amount/4; i++){
                hive[i] = new bee;
                hive_size += 1;
        }

        home = new bee_hive;  // create beehive

        Button2 -> Enabled = 0;
        Timer1 -> Enabled = 1;  // enable timer
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender){
         int i;
	
	// move all the bees
         for (i = 0; i < hive_size; i++){
                hive[i]->move();
         }

         home -> add_bee();  // check if beehive can create new bee
         
         Label2 -> Caption = home -> honey_hive;  // change caption
         Label4 -> Caption = hive_size;  // change caption
}
//---------------------------------------------------------------------------
