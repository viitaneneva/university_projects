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
# File: main_window.hh                                             #
# Description: Declares a class implementing a UI for the game.    #
#                                                                  #
# Author: Linnea Viitanen, 269085, linnea.viitanen@tuni.fi         #
####################################################################
*/

#ifndef PRG2_SNAKE2_MAINWINDOW_HH
#define PRG2_SNAKE2_MAINWINDOW_HH

#include "ui_main_window.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>
#include <random>
#include <QObject>


/* \class MainWindow
 * \brief Implements the main window through which the game is played.
 */
class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    /* \brief Construct a MainWindow.
     *
     * \param[in] parent The parent widget of this MainWindow.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /* \brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    /* \brief Change the Snake's bearing when certain keys get pressed.
     *
     * \param[in] event Contains data on pressed and released keys.
     */
    void keyPressEvent(QKeyEvent* event) override;

private slots:

    /* \brief Start the game.
     */
    void on_playButton_clicked();

    /* \brief Move the Snake by a square and check for collisions.
     *
     * The game ends if a wall or the Snake itself gets in the way.
     * When a food gets eaten a point is gained and the Snake grows.
     */
    void move_snake();

    /* \brief Tracks the playtime.
     */
    void play_time();


private:

    /* \brief Make the play field visible and fit it into the view.
     *
     * Should be called every time the field's size changes.
     */
    void adjustSceneArea();

    /* \brief Pauses the game when player presses the key P.
     *
     * Pauses the playtime tracker as well as the snake itself.
     */
    void pause_game();

    /* \brief Checks the score progress. When certain score is
     * reached, the speed of the snake increases.
     */
    void check_score();

    /* \brief If the snake has ran into its own body, or has hit
     * a wall, the game is over. Stops the timer and enables
     * the usage of the play button again.
     */
    void game_over();

    /* \brief Generates a random location for the food.
     */
    QPoint generate_location();

    /* \brief Check if the snake has eaten the food.
     *
     * If the food was eaten, the snake grows by one square.
     * \param[in] position: The point the snake is about to enter.
     */
    bool check_if_food_was_eaten(QPointF position);

    /* \brief Check if the snake has ran into its own body,
     * if it has hit a wall, or if the user tried to steer
     * the snake backwards (towards its neck).
     *
     * \param[in] snakes_old_position: the point snake's leaving from
     * \param[in] snakes_new_position: the point snake's entering
     * \param[in] food_status: Indicates if the food has been eaten or not
     * \param[in] active_points: List containing the points the snake is occupying
     * */
    bool check_for_special_cases(QPointF snakes_old_position,
                                 QPointF& snakes_new_position,
                                 QList<QPointF> active_points);

    /* \brief Move the food to a new location.
     *
     * \param[in] active_points: List containing the points the snake is occupying
     */
    void move_food(QList<QPointF> active_points);


    QList<QGraphicsRectItem*> snake_;   /**< Datastructure, that contains all the snake's points. */
    QGraphicsRectItem* food_ = nullptr; /**< The food item in the scene. */

    bool game_paused = true;            /**< Tracks if the game has been paused or not. */
    bool game_won = false;              /**< Tracks if the gameround has ended in a victory. */

    QPointF movement_point_;            /**< The default point to steer the snake. */

    Ui::MainWindow ui_;                 /**< Accesses the UI widgets. */

    QGraphicsScene scene_;              /**< Manages drawable objects. */

    QTimer timer_;                      /**< Triggers the Snake to move. */
    QTimer gametime_;                   /**< Tracks the game time. */

    std::default_random_engine rng_;    /**< Randomizes food locations. */

    int board_width_ = 100;             /**< The board width. */
    int board_height_ = 100;            /**< The board height. */

    int score_ = 0;                     /**< The current score. */
    int highscore_ = 0;                 /**< The best score so far. */

    int tries_ = 0;                     /**< Counts how many time the game's been played. */
    int wins_ = 0;                      /**< How many times the player has won. */
    int deaths_ = 0;                    /**< How many times the player has died. */

    int time_in_seconds = 0;            /**< Time spent in the current try. */

};  // class MainWindow


#endif  // PRG2_SNAKE2_MAINWINDOW_HH
