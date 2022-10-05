#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include<sstream>

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
int SCORE = 0 , score_counter = 1;
double TIME_PASSED = 0;
int HEALTH = 3;
int time_integer = 1 , t_p = 0;
int LEVEL = 1;

int move_bullets(vector<Bullet> &bullets, vector<Bubble> &bubbles , int &SCORE , int &LEVEL){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
        }
    for(unsigned int j = 0 ; j < bubbles.size() ; j++){
      for(unsigned int i=0; i<bullets.size(); i++){
        if(pow(bubbles[j].get_center_x()-bullets[i].get_center_x(), 2) + pow(bubbles[j].get_center_y()-bullets[i].get_center_y(), 2)<
             pow(bubbles[j].get_radius()+14.142, 2)){    //try writing the conditon using power function refer disc intersection problem
           if(bubbles[j].get_radius() == BUBBLE_DEFAULT_RADIUS){
               bullets.erase(bullets.begin()+i);
               bubbles.erase(bubbles.begin()+j);
               SCORE++;
               }
           else if(bubbles[j].get_radius() == 2*BUBBLE_DEFAULT_RADIUS){
               bullets.erase(bullets.begin()+i);
               bubbles.push_back(Bubble(bubbles[j].get_center_x()-10, bubbles[j].get_center_y(), BUBBLE_DEFAULT_RADIUS, -1.5*BUBBLE_DEFAULT_VX, 0, COLOR(143, 0, 255)));
               bubbles.push_back(Bubble(bubbles[j].get_center_x()+10, bubbles[j].get_center_y(), BUBBLE_DEFAULT_RADIUS, 1.5*BUBBLE_DEFAULT_VX, 0, COLOR(143, 0, 255)));
               bubbles.erase(bubbles.begin()+j);
               SCORE++;
               }
           }
        }
      }
return SCORE;
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles(int &LEVEL)
{
    vector<Bubble> bubbles;

    // create initial bubbles in the game for level 1
    if(LEVEL == 1){
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    }

    // create initial bubbles in the game for level 2
    else if(LEVEL == 2){
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, -1.5*BUBBLE_DEFAULT_VX, 0, COLOR(143, 0, 255)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, 1.5*BUBBLE_DEFAULT_VX, 0, COLOR(143, 0, 255)));
    bubbles.push_back(Bubble(3*WINDOW_X/4.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, 1.5*BUBBLE_DEFAULT_VX, 0, COLOR(143, 0, 255)));
    }
    return bubbles;
}


void level_1(){
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

 while(LEVEL < 3){
    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    //Text for game score
    string score_str("SCORE:0 ");
    Text game_score((LEFT_MARGIN+300), BOTTOM_MARGIN, score_str);

    //Text for TIME
    string time_str("TIME:0 /50");
    Text game_time(LEFT_MARGIN, TOP_MARGIN, time_str);

    //Text for HEALTH
    string health_str("HEALTH:3 /3");
    Text game_health((LEFT_MARGIN+300) , TOP_MARGIN , health_str);

    if(LEVEL == 2){
    stringstream stream;
        stream << SCORE;
        stream >> score_str[6];
        game_score.setMessage(score_str);
        stream << HEALTH;
        stream >> health_str[7];
        game_health.setMessage(health_str);
        }

    //Text for level display
    Text level((LEFT_MARGIN + 150), BOTTOM_MARGIN, "LEVEL:    /2");
    //Display level
    Text level_display((LEFT_MARGIN + 170), BOTTOM_MARGIN, LEVEL);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles(LEVEL);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    //level display
    if(LEVEL == 1){ Text level_display(250, 250, "LEVEL 1!");
    getClick();
    //wait(0.5);
    level_display.setMessage("");
    }
    else if(LEVEL == 2){ Text level_display(250, 250, "LEVEL 2!");
    getClick();
    //wait(0.5);
    level_display.setMessage("");
    }

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets, bubbles, SCORE, LEVEL);

        //Increment score
        if(SCORE == score_counter){ score_counter++;
        stringstream stream;
        stream << SCORE;
        stream >> score_str[6];
        stream >> score_str[7];
        game_score.setMessage(score_str);
        }

        //Breaking loop if bubbles.size() = 0
        if(bubbles.size() == 0){
        Text congratulate(250, 250, "CONGRATULATIONS!!!");
        getClick();
        break;
        }

        //Redcuing health for bubble collision
        for(size_t i = 0 ; i < bubbles.size() ; i++){
        if((abs(bubbles[i].get_center_x()-shooter.get_body_center_x()) < 9+bubbles[i].get_radius()
               &&
            abs(bubbles[i].get_center_y()-shooter.get_body_center_y()) < 15+bubbles[i].get_radius())
            ||
            pow(bubbles[i].get_center_x()-shooter.get_head_center_x(), 2) + pow(bubbles[i].get_center_y()-shooter.get_head_center_y(), 2)
           < pow(shooter.get_head_radius()+bubbles[i].get_radius(), 2)){
           HEALTH = HEALTH - 1;
           stringstream stream;
           stream << HEALTH;
           stream >> health_str[7];
           game_health.setMessage(health_str);
           while((abs(bubbles[i].get_center_x()-shooter.get_body_center_x()) < 9+bubbles[i].get_radius()
                 &&
                 abs(bubbles[i].get_center_y()-shooter.get_body_center_y()) < 15+bubbles[i].get_radius())
                 ||
                 pow(bubbles[i].get_center_x()-shooter.get_head_center_x(), 2) + pow(bubbles[i].get_center_y()-shooter.get_head_center_y(), 2)
                 < pow(shooter.get_head_radius()+bubbles[i].get_radius(), 2)){
                 if(HEALTH == 0) break;

                 // Update the bubbles
                 move_bubbles(bubbles);

                // Update the bullets
                move_bullets(bullets, bubbles, SCORE, LEVEL);

                //blinking shooter
                shooter.body.setColor(COLOR(255, 255, 0)); shooter.body.setFill(true);
                shooter.head.setColor(COLOR(255, 255, 0)); shooter.body.setFill(true);
                shooter.body.setColor(COLOR(0, 255, 0)); shooter.body.setFill(true);
                shooter.head.setColor(COLOR(0, 255, 0)); shooter.body.setFill(true);

                wait(STEP_TIME);
                }
           }
        }

        //break out of while loop if HEALTH == 0
        if(HEALTH == 0) break;

        //Time increment and display
        TIME_PASSED = TIME_PASSED + 1.5*STEP_TIME;
        t_p = TIME_PASSED;

        if(t_p >= time_integer){
        time_integer++;
        stringstream stream;
        stream << t_p;
        stream >> time_str[5];
        stream >> time_str[6];
        game_time.setMessage(time_str);
        }

        //Time over
        if(t_p >= 50) break;

        wait(STEP_TIME);

  }
  if(t_p >= 50 || HEALTH == 0){
   Text game_over(250 , 250 , "GAME OVER!");
   getClick();
   return;
   }

  //initializing variables for level 2
  if(bubbles.size() == 0){ LEVEL++;
    TIME_PASSED = 0; t_p = 0; time_integer = 1;
    }

  }
return;
}

int main(){
level_1();
}
