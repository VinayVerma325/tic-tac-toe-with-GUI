#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int WINDOW_SIZE = 600;
const int CELL_SIZE = WINDOW_SIZE / 3;
char board[3][3];
bool gameOver = false;
char winner = ' ';
int winType = 0;

void resetGame()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
    gameOver = false;
    winner = ' ';
    winType = 0;
}

char checkWinner()
{
    winType = 0;
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            winType = i + 1; return board[i][0];
        }
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            winType = i + 4; return board[0][i];
        }
    }
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        winType = 7; return board[0][0];
    }
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        winType = 8; return board[0][2];
    }
    bool full = true;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ') full = false;
    if (full) return 'D';
    return ' ';
}

void makeRandomMove()
{
    while (true)
    {
        int pos = rand() % 9;
        int row = pos / 3, col = pos % 3;
        if (board[row][col] == ' ')
        {
            board[row][col] = 'O';
            break;
        }
    }
}

int main()
{
    srand(time(0));
    resetGame();

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE + 130), "Tic Tac Toe");

    // Font (Windows ke liye correct path)
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        // Agar error aaye to yeh message console pe
        std::cout << "Font load failed! Make sure arial.ttf exists." << std::endl;
    }

    sf::Text resultText("", font, 38);
    resultText.setFillColor(sf::Color::Black);
    resultText.setPosition(110, WINDOW_SIZE + 12);

    sf::RectangleShape restartButton(sf::Vector2f(230, 58));
    restartButton.setPosition(185, WINDOW_SIZE + 62);
    restartButton.setFillColor(sf::Color(45, 160, 75));

    sf::Text restartBtnText("Restart", font, 28);
    restartBtnText.setFillColor(sf::Color::White);
    restartBtnText.setPosition(225, WINDOW_SIZE + 72);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                int mx = event.mouseButton.x, my = event.mouseButton.y;

                // Restart
                if (gameOver && mx >= 185 && mx <= 415 && my >= WINDOW_SIZE + 62 && my <= WINDOW_SIZE + 120)
                    resetGame();

                // Player move
                if (!gameOver && my < WINDOW_SIZE)
                {
                    int row = my / CELL_SIZE, col = mx / CELL_SIZE;
                    if (board[row][col] == ' ')
                    {
                        board[row][col] = 'X';
                        winner = checkWinner();
                        if (winner == ' ')
                        {
                            makeRandomMove();
                            winner = checkWinner();
                        }
                        if (winner != ' ') gameOver = true;
                    }
                }
            }
        }

        // Hover
        sf::Vector2i mp = sf::Mouse::getPosition(window);
        bool hovered = gameOver && mp.x >= 185 && mp.x <= 415 && mp.y >= WINDOW_SIZE + 62 && mp.y <= WINDOW_SIZE + 120;
        restartButton.setFillColor(hovered ? sf::Color(35, 130, 60) : sf::Color(45, 160, 75));

        window.clear(sf::Color(248, 248, 248));

        // Grid
        for (int i = 1; i < 3; i++)
        {
            sf::RectangleShape h(sf::Vector2f(WINDOW_SIZE, 9)); h.setPosition(0, i * CELL_SIZE - 4.5f); h.setFillColor(sf::Color(40, 40, 40)); window.draw(h);
            sf::RectangleShape v(sf::Vector2f(9, WINDOW_SIZE)); v.setPosition(i * CELL_SIZE - 4.5f, 0); v.setFillColor(sf::Color(40, 40, 40)); window.draw(v);
        }

        // Win Line (pehle draw karo taaki symbols ke neeche rahe)
        if (gameOver && winner != ' ' && winner != 'D')
        {
            sf::RectangleShape wl;
            wl.setFillColor(sf::Color(0, 200, 80, 200));
            if (winType >= 1 && winType <= 3) // row
            {
                int r = winType - 1;
                wl.setSize({ WINDOW_SIZE, 18 });
                wl.setPosition(0, r * CELL_SIZE + CELL_SIZE / 2 - 9);
            }
            else if (winType >= 4 && winType <= 6) // column
            {
                int c = winType - 4;
                wl.setSize({ 18, WINDOW_SIZE });
                wl.setPosition(c * CELL_SIZE + CELL_SIZE / 2 - 9, 0);
            }
            else if (winType == 7) // diag1
            {
                wl.setSize({ 855, 18 });
                wl.setPosition(0, 0);
                wl.setRotation(45);
                wl.setOrigin(0, 9);
            }
            else if (winType == 8) // diag2
            {
                wl.setSize({ 855, 18 });
                wl.setPosition(WINDOW_SIZE, 0);
                wl.setRotation(135);
                wl.setOrigin(855, 9);
            }
            window.draw(wl);
        }

        // X and O
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                float x = j * CELL_SIZE, y = i * CELL_SIZE, cx = x + CELL_SIZE / 2, cy = y + CELL_SIZE / 2;
                if (board[i][j] == 'O')
                {
                    sf::CircleShape c(CELL_SIZE / 2 - 38); c.setOrigin(c.getRadius(), c.getRadius());
                    c.setPosition(cx, cy); c.setFillColor(sf::Color::Transparent);
                    c.setOutlineThickness(19); c.setOutlineColor(sf::Color(20, 110, 210));
                    window.draw(c);
                }
                if (board[i][j] == 'X')
                {
                    float len = CELL_SIZE - 52, th = 19;
                    sf::RectangleShape l1({ len, th }), l2({ len, th });
                    l1.setOrigin(len / 2, th / 2); l1.setPosition(cx, cy); l1.setRotation(45); l1.setFillColor(sf::Color(210, 40, 40));
                    l2.setOrigin(len / 2, th / 2); l2.setPosition(cx, cy); l2.setRotation(135); l2.setFillColor(sf::Color(210, 40, 40));
                    window.draw(l1); window.draw(l2);
                }
            }

        // Result + Button
        if (gameOver)
        {
            if (winner == 'X') resultText.setString("You Win! 🎉");
            else if (winner == 'O') resultText.setString("Computer Wins!");
            else if (winner == 'D') resultText.setString("It's a Draw!");

            window.draw(resultText);
            window.draw(restartButton);
            window.draw(restartBtnText);
        }

        window.display();
    }
    return 0;
}