"""
Graafinen käyttöliittymä, skaalautuva
TIE-02100 Johdatus ohjelmointiin
Aleksi Raatala
Mauri Leino

Hirsipuu-tyyppinen peli kahdelle pelaajalle
Säännöt:

Pelissä toinen pelaaja syöttää syötekenttään sanan, jota toinen pelaaja
lähtee arvaamaan painamalla ikkunaan avautuvia kirjainnäppäimiä. Syöte ei saa
olla tyhjä, eikä se saa sisältää numeroita tai välilyöntejä.

Syöte hyväksytään painamalla syötekentän vieressä olevaa nappulaa. Jos
syötteessä on jotain vikaa, saa pelaaja antaa syötteen uudestaan. Ensimmäisellä
vuorolhila pelaaja 1 antaa sanan, ja pelaaja 2 arvaa.

Kun syöte on lukittu, muuttuvat sanan kirjaimet tähdiksi, ja jos arvaava
pelaaja arvaa kirjaimen oikein, kaikki ko. kirjaimet sanassa tulevat näkyviin.

Arvaaja saa tehdä 7 virhettä, ja kahdeksannesta vuoro päättyy. Jos pelaaja ei
saa sanaa oikein, saa sanan syöttänyt pelaaja yhden pisteen. Jos pelaaja taas
arvaa kaikki sanan kirjaimet oikein, saa hän kaksi pistettä.

Syötekentän vieressä on myös Reset-nappi, jota arvaaja voi painaa esimerkiksi
painettuaan vahingossa jotain kirjainnappia. Tällöin virheet nollataan, kuten
myös arvatut kirjaimet. Reset-napin painalluksesta pelaaja saa kuitenkin yhden
miinuspisteen, joten sitä kannattaa käyttää harkiten.

Peli pelataan kymmeneen pisteeseen, mutta tasapuolisuuden nimissä pelin
päättyminen tarkistetaan vain, kun pelaaja 1 on arvannut sanaa, jotta tällä
on yhtäläiset mahdollisuudet saavuttaa 10 pistettä. Myös tasapeli on siis
mahdollinen.
"""

from tkinter import *


HANGMANPICS = ["Hangman_start.gif", "Hangman_step1.gif", "Hangman_step2.gif",
               "Hangman_step3.gif", "Hangman_step4.gif", "Hangman_step5.gif",
               "Hangman_step6.gif", "Hangman_step7.gif", "Hangman_final.gif"]

PLAYERS = 2

ALPHABET = ["q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "å", "a", "s",
            "d", "f", "g", "h", "j", "k", "l", "ö", "ä", "z", "x", "c", "v",
            "b", "n", "m"]

NUMBERS = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0"]

GAME_END_PICS = ["Wins1.gif", "Wins2.gif", "Tie.gif"]

GAME_END_SCORE = 10


class HangmanGame:
    def __init__(self):
        self.__window = Tk()
        self.__window.title("Hangman -game")

        self.__turn = 1
        self.__mistakes = 0
        self.__word_to_guess = None
        self.__player_scores = [0] * PLAYERS

        self.__hangmanpics = []
        for picturefile in HANGMANPICS:
            picture = PhotoImage(file=picturefile)
            self.__hangmanpics.append(picture)

        self.__end_game_pics = []
        for picturefile in GAME_END_PICS:
            picture = PhotoImage(file=picturefile)
            self.__end_game_pics.append(picture)

        self.__letterbuttons = {}
        for letter in ALPHABET:
            button = Button(self.__window, text=letter, width=2,
                            command=lambda y=letter: self.keyboard_input(y))
            self.__letterbuttons[letter] = button

        for number in range(PLAYERS):
            Label(self.__window, text="Player {:1s} score:"
                  .format(str(number + 1))).grid(row=number + 15, column=11)

        self.__pointlabels =[]
        for number in range(PLAYERS):
            label = Label(self.__window)
            label.grid(row=number + 15, column=12)
            self.__pointlabels.append(label)

        self.__keyboard_info = Label(self.__window, text="Player " +
                                     str((self.__turn % 2)) +
                                     ", enter a word!")
        self.__word_entry = Entry()
        self.__word_to_guess = None
        self.__word_guessed = []

        self.__entrylabel = Label(self.__window, text="Enter word:")
        self.__StartgameButton = Button(self.__window, text="Start the game!",
                                        command=self.initialize_game)

        self.__pictureLabel = Label(self.__window)

        self.__keyboard_info.grid(row=3, column=11)
        self.__entrylabel.grid(row=17, column=0, columnspan=3)
        self.__word_entry.grid(row=17, column=4, columnspan=5)
        self.__StartgameButton.grid(row=17, column=9)
        self.__pictureLabel.grid(row=0, column=0, rowspan=16, columnspan=10)

        Button(self.__window, text="Quit", command=self.__window.destroy) \
            .grid(row=17, column=26)

    def initialize_game(self):
        """ Initializes game by getting the word entered by player in turn and
        turning it into asterisks to show in the disabled entry box. All
        letters in the entry are decapitalized
        :return: None
        """
        #   TODO: checkaa toimiiko ilman vuoron asettamista, sillä se tehdään
        #   sekä rakentajassa että start_new_game-metodissa
        #   self.__turn = 1
        self.__word_to_guess = self.__word_entry.get().lower()

        self.__word_guessed = list(
            map(lambda x: "*", range(len(self.__word_to_guess))))

        self.__keyboard_info.configure(text="Player " +
                                            str((self.__turn % 2) + 1)
                                            + ", guess letters!")

        self.__entrylabel.configure(text="Guess this word")
        self.__StartgameButton.configure(text="Reset", command=self.reset_turn)

        self.setup_keyboard()
        self.update_ui()
        self.check_entry()

    def reset_turn(self):
        """ Resets mistakes to zero, and discards all guessed letters from the
        word to guess. Also takes one point from the player guessing the word.
        :return:
        """
        self.__player_scores[self.__turn % 2] -= 1

        self.__word_guessed = list(
            map(lambda x: "*", range(len(self.__word_to_guess))))

        self.__mistakes = 0

        self.setup_keyboard()
        self.update_ui()

    def keyboard_input(self, key):
        """
        Takes keyboard input from the player guessing the word, disables the
        button and checks if the player has guessed right.
        :param str key: The letter on the button pressed
        :return: None
        """
        self.__letterbuttons[key].configure(state=DISABLED)

        for index in range(len(self.__word_to_guess)):

            if key == self.__word_to_guess[index]:
                self.__word_guessed[index] = key

        if key not in self.__word_to_guess:
            self.__mistakes += 1

        self.update_ui()
        self.check_end_of_turn()

    def setup_keyboard(self, turn_over=False):
        """ Sets up the keyboard in the interface. If turn is over, keyboard is
        disabled while player in turn enters the word to guess.
        :param turn_over: If turn is over, true. False by default
        :return: None
        """
        if turn_over:
            state = DISABLED

        else:
            state = NORMAL

        row = 4
        column = 12

        while row <= 6:
            if row == 4:
                for index in range(ALPHABET.index("a")):

                    self.__letterbuttons[ALPHABET[index]].grid(row=row,
                                                               column=column)
                    self.__letterbuttons[ALPHABET[index]]\
                        .configure(state=state)
                    column += 1

            if row == 5:
                for index in range(ALPHABET.index("a"), ALPHABET.index("z")):
                    self.__letterbuttons[ALPHABET[index]].grid(row=row,
                                                               column=column)
                    self.__letterbuttons[ALPHABET[index]]\
                        .configure(state=state)
                    column += 1

            if row == 6:
                column = 14

                for index in range(ALPHABET.index("z"),
                                   ALPHABET.index("m") + 1):
                    self.__letterbuttons[ALPHABET[index]].grid(row=row,
                                                               column=column)
                    self.__letterbuttons[ALPHABET[index]]\
                        .configure(state=state)
                    column += 1

            column = 12
            row += 1

    def update_ui(self):
        """ Updates labels and texts in the interface
        :return: None
        """
        self.__word_entry.configure(state=NORMAL)
        self.__word_entry.delete(0, END)
        self.__word_entry.insert(0, " ".join(self.__word_guessed))
        self.__word_entry.configure(state=DISABLED)
        self.__pictureLabel.configure(
            image=self.__hangmanpics[self.__mistakes])

        for i in range(len(self.__pointlabels)):
            self.__pointlabels[i].configure(text=self.__player_scores[i])
        self.__keyboard_info.configure(text="Player " +
                                            str((self.__turn % 2) + 1)
                                            + ", guess letters!")

    def check_end_of_turn(self):
        """
        Checks if player guessing word has either exceeded the number of
        mistakes allowed, or guessed the whole word.
        :return: None
        """
        if self.__mistakes == 8:
            self.__turn += 1
            self.__player_scores[self.__turn % 2] += 1
            self.end_turn()

        if "*" not in self.__word_guessed:
            if self.__turn % 2 == 0:
                self.__player_scores[0] += 2

            else:
                self.__player_scores[1] += 2
            self.__turn += 1
            self.end_turn()

        if self.__mistakes != 8 and "*" in self.__word_guessed:
            self.update_ui()

    def start_turn(self):
        """
        Starts the turn for player guessing word given by other player.
        :return: None
        """
        self.__mistakes = 0
        self.__pictureLabel.configure(
            image=self.__hangmanpics[self.__mistakes])
        self.__word_to_guess = self.__word_entry.get().lower()

        self.__word_guessed = list(
            map(lambda x: "*", range(len(self.__word_to_guess))))
        #   self.__keyboard_info.grid(row=3, column=11)
        self.__entrylabel.configure(text="Guess this word")
        self.__StartgameButton.configure(text="Reset", command=self.reset_turn)
        self.setup_keyboard()
        self.update_ui()
        self.check_entry()

    def end_turn(self, text="Enter word:"):
        """ Ends current turn by clearing entry box, but doesn't change the
        player in turn. If player 1 has finished guessing, points of players
        are checked for a possible winner.
        :return: None
        """
        self.__word_entry.configure(state=NORMAL)
        self.__word_entry.delete(0, END)
        self.__StartgameButton.configure(text="Start turn",
                                         command=self.start_turn)
        self.__entrylabel.configure(text=text)

        for i in range(len(self.__pointlabels)):
            self.__pointlabels[i].configure(text=self.__player_scores[i])
        self.setup_keyboard(turn_over=True)
        self.__keyboard_info.configure(text="Player " +
                                            str((lambda x: 1 if (x % 2) == 1
                                                else 2)(self.__turn)) +
                                            ", enter a word!")
        if self.__turn % 2 == 1:
            self.check_end_of_game()

    def check_entry(self):
        """ Checks if the entry given is acceptable
        :return: None
        """
        if not self.__word_to_guess:
            #   self.__entrylabel.configure(text="No entry!")
            self.end_turn("No entry!")

        for character in self.__word_to_guess:
            if character in NUMBERS:
                #   self.__entrylabel.configure(text="A number in entry!")
                self.end_turn("No numbers allowed!")

        if " " in self.__word_to_guess:
            #   self.__entrylabel.configure(text="No spaces allowed!")
            self.end_turn("No spaces allowed!")

    def check_end_of_game(self):
        """
        Checks if both or one of the players has exceeded the end game -score
        of ten points. If so, game is ended
        :return: None
        """
        if self.__player_scores[0] >= GAME_END_SCORE and \
                self.__player_scores[1] >= GAME_END_SCORE:
            self.end_game(2)

        elif self.__player_scores[0] >= GAME_END_SCORE:
            self.end_game(0)

        elif self.__player_scores[1] >= GAME_END_SCORE:
            self.end_game(1)

    def end_game(self, winner):
        """
        Ends the game and shows the winner in the picture label
        :param int winner: 0, 1 or 2. Stands for the index of game ending pic
        in the list they are stored in
        :return: None
        """
        self.setup_keyboard(True)
        self.__pictureLabel.configure(image=self.__end_game_pics[winner])
        self.__StartgameButton.configure(text="Start new game",
                                         command=self.start_new_game)
        self.__entrylabel.configure(text="")
        self.__keyboard_info.configure(text="")
        self.__word_entry.configure(state=DISABLED)

    def start_new_game(self):
        """ Starts new game by clearing points, clearing the entry box and
        normalizing it, sets the player in turn
        :return: None
        """
        self.__turn = 1
        self.__player_scores = [0] * PLAYERS
        self.__word_entry.configure(state=NORMAL)
        self.__word_entry.delete(0, END)
        self.__pictureLabel.configure(
            image=self.__hangmanpics[self.__mistakes])
        self.__entrylabel.configure(text="Enter word:")
        self.__keyboard_info.configure(text="Player " + str((self.__turn % 2))
                                                      + ", enter a word!")
        self.__StartgameButton.configure(text="Start the game!",
                                         command=self.initialize_game)

    def start(self):
        self.__window.mainloop()


def main():
    ui = HangmanGame()
    ui.start()


main()
