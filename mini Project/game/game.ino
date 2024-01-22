#include <U8glib.h>
//#include <Wire.h>

// Initialize the OLED display
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

// Joystick pin
int yPin = A1;

// Paddle sizes and positions
int playerPaddleY = 30;
int aiPaddleY = 30;
int paddleHeight = 25;
int paddleWidth = 4;

// Ball position, speed, and size
int ballX = 64, ballY = 32;
int ballSpeedX = 3, ballSpeedY = 3;
int ballSize = 5;

// Scoring and game state
int playerScore = 0;
bool gameOverFlag = true;
char finish = 'H';

void setup() {
  Serial.begin(115200);
  pinMode(yPin, INPUT);
}

void loop() {
  if (Serial.available()){
    char data =Serial.read();
    if (data=='G'&& finish=='H')
    {
      resetGame();
      finish='A';
    }
  }
  Serial.println(finish);
  if (gameOverFlag) {
    displayGameOver(); 
    if(finish=='A'){
      finish = 'F';
    }
    return;
  }

  int yValue = analogRead(yPin);

  // Move player paddle based on joystick
  if (yValue < 400) playerPaddleY -= 3;
  if (yValue > 600) playerPaddleY += 3;
  playerPaddleY = constrain(playerPaddleY, 0, 64 - paddleHeight);

  // Simple AI for opponent paddle
  aiPaddleY = ballY - paddleHeight / 2;
  aiPaddleY = constrain(aiPaddleY, 0, 64 - paddleHeight);

  // Move ball
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // Collision with top and bottom
  if (ballY <= 0 || ballY >= 64 - ballSize) ballSpeedY = -ballSpeedY;

  // Collision with paddles
  if (ballX <= paddleWidth && ballY >= playerPaddleY && ballY <= playerPaddleY + paddleHeight) {
    ballSpeedX = -ballSpeedX;
    playerScore++;
    increaseDifficulty();
  }
  if (ballX >= 128 - paddleWidth - ballSize && ballY >= aiPaddleY && ballY <= aiPaddleY + paddleHeight) ballSpeedX = -ballSpeedX;

  // Check if the player misses the ball
  if (ballX < 0) {
    gameOverFlag = true;
  }

  // Reset ball if it goes off screen to the right
  if (ballX > 128 - ballSize) {
    ballX = 64;
    ballY = 32;
    ballSpeedX = -ballSpeedX;
  }

  // Drawing
  u8g.firstPage();
  do {
    drawGame();
  } while (u8g.nextPage());
}

void drawGame() {
  // Draw player paddle
  u8g.drawBox(0, playerPaddleY, paddleWidth, paddleHeight);

  // Draw AI paddle
  u8g.drawBox(128 - paddleWidth, aiPaddleY, paddleWidth, paddleHeight);

  // Draw ball
  u8g.drawBox(ballX, ballY, ballSize, ballSize);

  // Draw score
  u8g.setPrintPos(60, 10);
  u8g.print(playerScore);
}

void displayGameOver() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(40, 32, "GAME OVER");
    u8g.setPrintPos(40, 48);
    u8g.print("Score: ");
    u8g.print(playerScore);
  } while (u8g.nextPage());
}

void resetGame() {
  playerPaddleY = 30;
  aiPaddleY = 30;
  ballX = 64;
  ballY = 32;
  ballSpeedX = 3;
  ballSpeedY = 3;
  playerScore = 0;
  gameOverFlag = false;
}

void increaseDifficulty() {
  if (playerScore % 5 == 0) {  // Increase speed every 5 points
    ballSpeedX;
  }
}
