/*
####################################################################
# TIE-02201 Ohjelmointi 2: Perusteet, K2019                        #
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project4: Snake: The Revengeance                                 #
# Program description: "While revenge is served cold and vengeance #
#                       is served hot revengeance is often seen    #
#                       served with Cajun with a side of onion     #
#                       rings and often dumplings in the Orient."  #
#                       - urbandictionary                          #
#                                                                  #
# File: main_window.cpp                                            #
# Description: Defines a class implementing a UI for the game.     #
#                                                                  #
# Author: Linnea Viitanen, 269085, linnea.viitanen@tuni.fi         #
####################################################################
*/

#include "main_window.hh"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QDebug>
#include <QList>
#include <QImage>

MainWindow::MainWindow(QWidget* parent):

    QMainWindow(parent) {

    ui_.setupUi(this);
    this->setStyleSheet("background-color: rgb(31,31,35)");
    ui_.graphicsView->setScene(&scene_);

    ui_.pause_label->setVisible(false);

    // Change the default widget colors to match the UI
    ui_.score_display->setStyleSheet("background-color: black");
    ui_.playtime_display->setStyleSheet("background-color: black");
    ui_.playButton->setStyleSheet("background-color: rgb(25,25,35); color: white");
    ui_.gamestatus_label->setStyleSheet("QLabel { color : white }");
    ui_.pause_label->setStyleSheet("QLabel { color : white }");
    ui_.time_label->setStyleSheet("QLabel { color : white }");
    ui_.highscore_label->setStyleSheet("QLabel { color : white }");
    ui_.score_label->setStyleSheet("QLabel { color : white }");
    ui_.amount_of_deaths_label->setStyleSheet("QLabel { color : white }");
    ui_.amount_of_wins_label->setStyleSheet("QLabel { color : white }");

    // The gameboard color
    scene_.setBackgroundBrush(QBrush(Qt::black));

    connect(&timer_, &QTimer::timeout, this, &MainWindow::move_snake);
    connect(&gametime_, &QTimer::timeout, this, &MainWindow::play_time);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    if( event->key() == Qt::Key_W){
        movement_point_.setX(0);
        movement_point_.setY(-10);
    }
    else if( event->key() == Qt::Key_A){
        movement_point_.setX(-5);
        movement_point_.setY(0);

    }
    else if( event->key() == Qt::Key_S){
        movement_point_.setX(0);
        movement_point_.setY(10);

    }
    else if( event->key() == Qt::Key_D){
        movement_point_.setX(5);
        movement_point_.setY(0);
    }

    else if(event->key() == Qt::Key_P){
        pause_game();
    }
}

void MainWindow::play_time(){

    // Track the play time and display it to the user
    ++time_in_seconds;
    ui_.playtime_display->display(time_in_seconds);
}

QPoint MainWindow::generate_location(){

    int i = 0;
    while(true){

        std::uniform_int_distribution<int> width_dist(0, board_width_);
        std::uniform_int_distribution<int> height_dist(0, board_height_);

        // Multiply the random integers by numbers so they wont go to
        // squares on the board that the snake cant access
        int new_width = width_dist(rng_)*5;
        int new_height = (height_dist(rng_)*10);

        if(new_width < board_width_ && new_height < board_height_){
            return QPoint(new_width, new_height);
        }
        ++i;
    }

}


void MainWindow::check_score(){

    // The starting speed is 0.5sec
    // The speed increases when a certain case
    // is activated

    switch(score_) {

    case 5: {timer_.setInterval(400);
                ui_.gamestatus_label->setText("LEVEL 2");
                break;}

    case 15: {timer_.setInterval(300);
                ui_.gamestatus_label->setText("LEVEL 3");
                break;}

    case 25: {timer_.setInterval(200);
                ui_.gamestatus_label->setText("LEVEL 4");
                break;}

    case 30: {timer_.setInterval(100);
                ui_.gamestatus_label->setText("LEVEL 5");
                break;}
  }
}

void MainWindow::on_playButton_clicked() {

    tries_ += 1;

    // Remove all objects from the scene if theres any,
    // before starting the game.
    if (tries_ > 1){
        int size = snake_.count();
        for(int i = 0; i < size; i++){
            delete(snake_.takeAt(0));
        }

        food_->setVisible(true);
        delete(food_);
    }

    // The gametime starts tracking
    time_in_seconds = 0;
    ui_.playtime_display->display(time_in_seconds);
    ui_.score_display->display(score_);
    gametime_.start(1000);


     // The game starts from level 1, gradually going up to level 5.
     ui_.gamestatus_label->setText("LEVEL 1");
     ui_.playButton->setDisabled(true);
     ui_.playButton->setStyleSheet("background-color: rgb(25,25,35); color: grey");

     // Set the direction where the snake is heading when the game starts.
     movement_point_.setX(-5);
     movement_point_.setY(0);

    // Generate and add a food-item to the gameboard.
    const QRectF food_rect(0, 0, 5, 10);
    QImage gem = QImage(":/myfiles/purplegem.png");
    food_ = scene_.addRect(food_rect, QPen(Qt::NoPen), QBrush(gem));

    // Set the food item starting point
    food_->setPos(generate_location());

    // Generate the first piece of the snake, and add it to the gameboard.
    QRectF snake_head(0, 0, 5, 10);
    const QPen snake_head_border(QColor(184, 0, 255, 255), 0);
    QGraphicsRectItem* new_part = scene_.addRect(snake_head,
                                                 snake_head_border,
                                                 (QColor(184, 0, 255, 150)));

    // Add the snake piece to the datastructure
    snake_.push_back(new_part);

    // Set the snake's starting point (at the center of the board)
    snake_.at(0)->setPos((board_width_/2), (board_height_/2));

    adjustSceneArea();
    timer_.start(500); // starting speed (0.5s)

}

void MainWindow::game_over(){

    // Stop both of the timers.
    timer_.stop();
    gametime_.stop();


    // Check whether the game ended in victory or defeat, and act accordingly.
    if (game_won){
            wins_ += 1;
            ui_.gamestatus_label->setText("GAME WON!");
            ui_.amount_of_wins_label->setText("Wins: " + QString::number(wins_));
        }
    else{
        deaths_ += 1;
        ui_.gamestatus_label->setText("GAME OVER");
        ui_.amount_of_deaths_label->setText("DEATHS: " + QString::number(deaths_));
    }

    // Make the gamestatus-text bold, so it stands out.
    QFont font = ui_.gamestatus_label->font();
    font.setBold(true);
    ui_.gamestatus_label->setFont(font);

    // If the player's final score is better than the current highscore,
    // the highscore will be adjusted.
    if(score_ > highscore_){
        highscore_ = score_;
        ui_.highscore_label->setText("HIGHSCORE: "+ QString::number(score_));
    }

    // Reset the score.
    score_ = 0;

    // Re-enable the play button for usage.
    ui_.playButton->setDisabled(false);
    ui_.playButton->setStyleSheet("background-color: rgb(25,25,35); color: white");
}


bool MainWindow::check_if_food_was_eaten(QPointF snake_new_pos){

    // The size of the snake-datastructure
    std::vector<QGraphicsRectItem*>::size_type snake_size = snake_.size();

    // Check if the snake's entering a point where the food is
    if (snake_new_pos == food_->scenePos()){

        // One food equals 1 point
        score_ += 1;
        ui_.score_display->display(score_);

        // Set the color schemes for the new piece
        QPen snake_body_border(QColor(184, 0, 255, 255), 0);
        QColor snake_color;

        // The color of the new piece is a bit darker than the one before,
        // to some point. (If the RGB value reaches 0,0,0, the rest of the
        // snake would be black.)
        if (snake_size <= 30){
            snake_color = QColor(137-(snake_size*3), 0, 204-(snake_size*3), 90);

        }
        else{
            snake_color = QColor(37, 0, 104, 90);
        }

        // Create the new piece and add it to the scene.
        QRectF snake_rect = QRectF(0.25, 0.5, 4.5, 9);
        QGraphicsRectItem* new_part = scene_.addRect(snake_rect,
                                                     snake_body_border,
                                                     snake_color);

        new_part->setPos(snake_.at(snake_size-1)->scenePos());

        // Add the piece to the datastructure.
        snake_.push_back(new_part);

        return true;

    }
    else{
        // If the snake's not entering a point where the food is
        return false;
    }
}

bool MainWindow::check_for_special_cases(QPointF snake_old_pos,
                                         QPointF& snake_new_pos,
                                         QList<QPointF> active_points){

    // The size of the snake-datastructure
    std::vector<QGraphicsRectItem*>::size_type snake_size = snake_.size();

    // Check if the player tried to change the direction 180 degrees
    if(snake_size > 1 && snake_new_pos == snake_.at(1)->scenePos()){

        movement_point_.setX(-1*movement_point_.x());
        movement_point_.setY(-1*movement_point_.y());

        snake_new_pos = snake_old_pos + movement_point_;
    }

    // Check if the snake ran into a wall
    if ((snake_new_pos.x() < 0 || snake_new_pos.x() == board_width_) ||
        (snake_new_pos.y() < 0 || snake_new_pos.y() == board_height_)) {

        game_over();
        return true;

    }

    // Check if the snake ran into its own body
    if(std::find(active_points.begin(),active_points.end(),snake_new_pos)
            != active_points.end()){

        if(snake_new_pos != snake_.at(1)->scenePos()){
            game_over();
            return true;
        }
    }

    // If none of the above cases apply
    return false;
}

void MainWindow::move_food(QList<QPointF> active_points){

    // The point where the food is currently
    QPointF food_old_point = food_->scenePos();

    // Search for a free point
    while (true) {
        QPoint new_food_location = generate_location();

        if (new_food_location != food_old_point){
            if (!(std::find(active_points.begin(),
                            active_points.end(),
                            new_food_location) != active_points.end())){

                // Snakeless square found, stop moving the food around.
                if (new_food_location != snake_.at(1)->scenePos()){
                    food_->setPos(new_food_location);
                    break;
                }
            }
        }
    }
}

void MainWindow::move_snake() {

    // The point where the snake's head currently is
    QPointF snake_old_point = snake_.at(0)->scenePos();

    // The point where the snake's head is entering
    QPointF snake_new_point = snake_old_point + movement_point_;

    // Temporary store for a point
    QPointF temporary;

    // The size of the snake-datastructure
    std::vector<QGraphicsRectItem*>::size_type snake_size = snake_.size();

    // Create a list of the points the snake's body is occupying
    // (Doesn't include the head)
    QList<QPointF> active_points;
    for (unsigned int i = 1; i < snake_size-1; ++i){
        active_points.append(snake_.at(i)->scenePos());
    }

    // Check if there are any problems with the point the snake's head
    // is entering
    if(check_for_special_cases(snake_old_point,
                               snake_new_point,
                               active_points)){
        return;
    }

    // Check if the snake's entering a point where the food is
    bool food_status = check_if_food_was_eaten(snake_new_point);

    // Move the snake. The snake's head will go to the new point,
    // and the rest of its body will follow the old points.
    for(unsigned int i = 0; i < snake_size; ++i){
        if (i == 0){
            snake_.at(0)->setPos(snake_new_point);
        }
        else{
            temporary = snake_.at(i)->scenePos();
            snake_.at(i)->setPos(snake_old_point);
            snake_old_point = temporary;
        }
    }

    // Place the food into a new, random point, if it was eaten.
    if(food_status){

        unsigned int board_area = ((board_height_/2)*board_width_);

        // If the snake's occupying all of the points (= the game's won),
        // the food will disappear from the board.
        if(snake_size == board_area){
            food_->setVisible(false);
            game_won = true;
            game_over();
        }
        else{
            move_food(active_points);
        }

    }

    // Clear the list
    active_points.clear();

    // Check the current score, and change the speed of the snake,
    // if necessary.
    check_score();

}

void MainWindow::pause_game() {

    // If the player presses the key "P",
    // the game pauses
    if (game_paused == true){

        ui_.gamestatus_label->setVisible(false);
        ui_.pause_label->setVisible(true);

        game_paused = false;

        // Stop both of the timers.
        gametime_.stop();
        timer_.stop();

    }

    // If the player presses the key "P" again,
    // the game unpauses
    else if (game_paused == false){

        ui_.gamestatus_label->setVisible(true);
        ui_.pause_label->setVisible(false);

        game_paused = true;

        // Restart both of the timers.
        gametime_.start();
        timer_.start();
    }
}

void MainWindow::adjustSceneArea() {

    const QRectF area(0, 0, board_width_, board_height_);
    scene_.setSceneRect(area);
    ui_.graphicsView->fitInView(area);
}
