#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>


enum State {
    DISPLAY,
    NOTDISPLAY,
    MINE,

};
enum Stateflag {
    NOFLAG,
    FLAG
};

// Structure pour une cellule de la pile
struct StackNode {
    int row;
    int col;
};

struct Stack {
    StackNode* nodes;
    int capacity;
    int top;

    Stack(int size) {
        capacity = size;
        nodes = new StackNode[size];
        top = -1;
    }

    ~Stack() {
        delete[] nodes;
    }

    void push(int row, int col) {
        if (top < capacity - 1) {
            top++;
            nodes[top].row = row;
            nodes[top].col = col;
        }
    }

    StackNode pop() {
        if (top >= 0) {
            return nodes[top--];
        }
        return { -1, -1 }; // Valeur par défaut si la pile est vide
    }

    bool isEmpty() {
        return top == -1;
    }
};

void clear() {
    // system("clear"); // Linux
    system("cls"); // Windows
}

struct Grid {
    State** values;
    Stateflag** flag;
    int width;
    int height;
    int nbmines;
    int nbflag;


    bool First_Attempt;
    bool defeat;


    Grid() : defeat(false), First_Attempt(true), values(nullptr), flag(nullptr), width(0), height(0), nbmines(0) {}

    ~Grid() {
        // Libération de la mémoire allouée pour les flags
        if (flag) {
            for (int row = 0; row < height; ++row) {
                delete[] flag[row];
            }
            delete[] flag;
        }

        // Libération de la mémoire allouée pour les valeurs
        if (values) {
            for (int row = 0; row < height; ++row) {
                delete[] values[row];
            }
            delete[] values;
        }
    }

    void reset()
    {

        if (flag) {
            for (int row = 0; row < height; ++row) {
                delete[] flag[row];
            }
            delete[] flag;
        }

        // Libération de la mémoire allouée pour les valeurs
        if (values) {
            for (int row = 0; row < height; ++row) {
                delete[] values[row];
            }
            delete[] values;
        }

        defeat = false;
        First_Attempt = true;
        values = nullptr;
        flag = nullptr;
        width = 0;
        height = 0;
        nbmines = 0;

    }


    void create(int width_, int height_) {
        width = width_;
        height = height_;


        flag = new Stateflag * [height];
        for (int row = 0; row < height; ++row)
            flag[row] = new Stateflag[width];
        for (int row = 0; row < height; ++row)
            for (int col = 0; col < width; ++col)
                flag[row][col] = NOFLAG;

        values = new State * [height];
        for (int row = 0; row < height; ++row)
            values[row] = new State[width];
        for (int row = 0; row < height; ++row)
            for (int col = 0; col < width; ++col)
                values[row][col] = NOTDISPLAY;


    }

    void placeMines(int safeRow, int safeCol) {

        srand(static_cast<unsigned int>(time(0))); // Initialiser le générateur de nombres aléatoires
        int placedMines = 0;

        while (placedMines < nbmines) {
            int row = rand() % height;
            int col = rand() % width;

            // Vérifier que la mine n'est pas placée dans la première case ou ses voisines
            if (values[row][col] != MINE && !(row == safeRow && col == safeCol) && First_Attempt) {
                values[row][col] = MINE;
                placedMines++;
            }
        }
    }

    void RowNbIndication(int row)
    {
        if (row < 9 && row > 0)
            std::cout << " " << row + 1 << " ";

        else if (row < 99 && row >= 9)
            std::cout << row + 1 << " ";

        else if (row == 99)
            std::cout << row + 1 << "";
    }
    void ColNbIndication(int col)
    {

        if (col < 9 && col > 0)
            std::cout << col + 1 << "     ";

        else if (col < 99 && col >= 8)
            std::cout << col + 1 << "    ";

        else if (col == 99)
            std::cout << col + 1 << "";
    }


    bool WinCondition()
    {
        int NbDisplayAndMine = 0;
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                if (values[row][col] == DISPLAY || values[row][col] == MINE)
                    NbDisplayAndMine++;
            }
        }
        return ((width * height) == NbDisplayAndMine);

    }


    void show() {

        bool first = true;

        for (int row = 0; row < height; ++row)
        {
            RowNbIndication(row);
            for (int col = 0; col < width; ++col)
            {

                if (col == 0 && first)
                {
                    std::cout << "     " << 1 << "     ";
                    for (int i = 0; i <= width - 1; ++i)
                        ColNbIndication(i);

                    std::cout << std::endl;
                    first = false;
                    std::cout << " " << row + 1 << " ";
                }

                std::cout << "[" << " ";


                if (flag[row][col] == FLAG && values[row][col] != DISPLAY)
                    std::cout << "F" << " ";
                else if (values[row][col] == NOTDISPLAY)
                    std::cout << "-" << " ";
                else if (WinCondition() && (values[row][col] == MINE))
                    flag[row][col] = FLAG;
                else if (values[row][col] == MINE && defeat)
                    std::cout << "*" << " ";

                else if (values[row][col] == MINE)
                    std::cout << "-" << " ";
                else if (values[row][col] == DISPLAY)
                {
                    if (getNumberOfAdjacentMine(row, col) == 0)
                        std::cout << " " << " ";
                    else
                        std::cout << getNumberOfAdjacentMine(row, col) << " ";
                }
                std::cout << "]" << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "\n\n";
        std::cout << "Vous avez encore : " << nbflag << " flag\n";
    }

    int getNumberOfAdjacentMine(int cellRow, int cellCol)
    {
        int nbNeighboors = 0;
        for (int row = std::max(cellRow - 1, 0); row <= std::min(cellRow + 1, height - 1); ++row)
            for (int col = std::max(cellCol - 1, 0); col <= std::min(cellCol + 1, width - 1); ++col)
                if (values[row][col] == MINE)
                    nbNeighboors++;
        return nbNeighboors;
    }

    void floodFill(int startRow, int startCol)
    {
        if (values[startRow][startCol] == DISPLAY)
        {
            return;
        }

        Stack stack(width * height); // Créer une pile
        stack.push(startRow, startCol);
        values[startRow][startCol] = DISPLAY;

        while (!stack.isEmpty()) {
            StackNode node = stack.pop();
            int row = node.row;
            int col = node.col;

            if (getNumberOfAdjacentMine(row, col) == 0) {
                for (int r = std::max(row - 1, 0); r <= std::min(row + 1, height - 1); ++r) {
                    for (int c = std::max(col - 1, 0); c <= std::min(col + 1, width - 1); ++c) {
                        if (values[r][c] == NOTDISPLAY) {
                            values[r][c] = DISPLAY;
                            stack.push(r, c); // Ajouter à la pile
                        }
                    }
                }
            }
        }
    }



};



int main() {
    Grid grid;
    int largeur=0;
    int hauteur=0;
    bool Itsgood = false;

    int nbminesMax = 0; 
    int mines = 0;

    while (!Itsgood)
    {
        std::string difficulty;
        std::cout << "veuillez choisir une difficulte entre : easy | normal | hard | perso : ";
        std::cin >> difficulty;

        if (difficulty == "easy")
        {
            grid.create(9, 9);
            mines = 10;
            Itsgood = true;
        }

        else if (difficulty == "normal")
        {
            grid.create(16, 16);
            mines = 40;
            Itsgood = true;
        }

        else if (difficulty == "hard")
        {
            grid.create(29, 16);
            mines = 99;
            Itsgood = true;
        }

        else if (difficulty == "perso")
        {

            while (!Itsgood)
            {

                std::cout << "\nSaisissez la hauteur de la grille : ";
                std::cin >> hauteur;
                std::cout << "\nSaisissez la largeur de la grille : ";
                std::cin >> largeur;
                std::cout << "\nSaisissez le nombre de mines : ";
                std::cin >> mines;
                nbminesMax= (hauteur * largeur * 25) / 100;
                if ((hauteur <= 100 || largeur <= 100) && mines <= nbminesMax)
                {
                    grid.create(largeur, hauteur);
                    Itsgood = true;
                }
                else
                    if (mines > nbminesMax)
                    {
                        std::cout << "le nombre de mine ne doit pas etre superieur a : " << nbminesMax << "\n";
                    }
                    std::cout << "\nErreur\n";


            }

        }
        else
            std::cout << "\nErreur nom de difficulté\n";
    }
    grid.nbmines = mines;
    grid.nbflag = mines;


    while (true) {
        int x, y;
        std::string restart;
        std::string flag;
        Itsgood = false;



        grid.show();

        Itsgood = false;
        while (!Itsgood)
        {
            std::cout << "\nVoulez vous restart (y/n) : \n";
            std::cin >> restart;
            if (restart == "Y" || restart == "y")
            {
                clear();
                grid.reset();
                while (!Itsgood)
                {
                    std::string difficulty;
                    std::cout << "veuillez choisir une difficulté entre : easy | normal | hard | perso : ";
                    std::cin >> difficulty;

                    if (difficulty == "easy")
                    {
                        grid.create(9, 9);
                        mines = 10;
                        Itsgood = true;
                    }

                    else if (difficulty == "normal")
                    {
                        grid.create(16, 16);
                        mines = 40;
                        Itsgood = true;
                    }

                    else if (difficulty == "hard")
                    {
                        grid.create(29, 16);
                        mines = 99;
                        Itsgood = true;
                    }

                    else if (difficulty == "perso")
                    {

                        while (!Itsgood)
                        {

                            std::cout << "\nSaisissez la hauteur de la grille : ";
                            std::cin >> hauteur;
                            std::cout << "\nSaisissez la largeur de la grille : ";
                            std::cin >> largeur;
                            std::cout << "\nSaisissez le nombre de mines : ";
                            std::cin >> mines;
                            nbminesMax = (hauteur * largeur * 25) / 100;
                            if ((hauteur <= 100 || largeur <= 100) && mines <= nbminesMax)
                            {
                                grid.create(largeur, hauteur);
                                Itsgood = true;
                            }
                            else
                                if (mines > nbminesMax)
                                {
                                    std::cout << "le nombre de mine ne doit pas etre superieur a : " << nbminesMax << "\n";
                                }
                            std::cout << "\nErreur\n";


                        }

                    }
                    else
                        std::cout << "\nErreur nom de difficulté\n";
                }
                grid.nbmines = mines;
                grid.nbflag = mines;
                clear();
                grid.show();
            }
            else if (restart == "N" || restart == "n")
                Itsgood = true;
            else
                std::cout << "Erreur restart";
        }


        Itsgood = false;
        while (!Itsgood)
        {
            std::cout << "\nSaisir x : \n";
            std::cin >> x;
            std::cout << "Saisir y : \n";
            std::cin >> y;
            if (x > largeur || y > hauteur)
            {
                y--;
                x--;
                Itsgood = true;
            }

            else
                std::cout << "\nErreur coordonné\n";

            
        }


        Itsgood = false;
        while (!Itsgood)
        {
            std::cout << "\nEst ce que vous voulez mettre un drapeau ? (y/n) :\n";
            std::cin >> flag;
            if (flag == "N" || flag == "n" || flag == "Y" || flag == "y")
                Itsgood = true;
            else
                std::cout << "Erreur flag";
        }
        






        if (grid.First_Attempt) {
            // Placer les mines après le premier coup
            grid.placeMines(y, x);
            grid.First_Attempt = false; // Indique que le premier coup a été effectué
        }
        grid.show();



        if (flag == "Y" || flag == "y") {
            if (grid.flag[y][x] == NOFLAG)
            {
                grid.flag[y][x] = FLAG;
                grid.nbflag--;
            }

            else
            {
                grid.flag[y][x] = NOFLAG;
                grid.nbflag++;
            }


            grid.show();
        }
        else {
            if (grid.values[y][x] == MINE && grid.flag[y][x] == NOFLAG) 
            {
                std::cout << "\nTu as perdu";
                grid.defeat = true;
            }
            else if (grid.flag[y][x] == NOFLAG) {
                grid.floodFill(y, x);
            }


        }


        grid.show();
        clear();
        if (grid.WinCondition())
        {
            grid.show();
            std::cout << "\nTu as gagne";
            break;
        }
    }

}
