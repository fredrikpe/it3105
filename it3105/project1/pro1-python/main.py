import sys

from PyQt5 import QtWidgets, QtCore, uic

from algorithms.solver import Solver
from board import Board

Ui_MainWindow, QtBaseClass = uic.loadUiType("main_window.ui")


class MyApp(QtWidgets.QMainWindow, Ui_MainWindow):  #, Ui_MainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)

        self.board = Board(self)
        self.solver = Solver(self.board)


        self.main_grid_layout = self.gridLayout_2  # type: QtWidgets.QGridLayout
        self.main_grid_layout.addWidget(self.board, 2, 0, 1, 7)

        self.step_iterator = iter([])

        self.radioButton_BT.click()

    def update_step_iterator(self):
        self.on_queensLineEdit_textChanged(None)

    @QtCore.pyqtSlot(int)
    def on_boardSizeSpinBox_valueChanged(self, size):
        self.board.board_size = size
        self.on_queensLineEdit_textChanged(None)
        self.board.update()

    @QtCore.pyqtSlot(str)
    def on_queensLineEdit_textChanged(self, text):
        if not text:
            text = self.queensLineEdit.text()
        self.board.queens = [int(c) for c in text]
        self.solver.input_length = len(text)

        self.step_iterator = self.solver.solve(self.board.queens)
        self.board.update()

    @QtCore.pyqtSlot()
    def on_nextStepButton_clicked(self):
        try:
            self.board.queens = next(self.step_iterator)
        except StopIteration:
            self.update_step_iterator()
        self.board.update()

    @QtCore.pyqtSlot(bool)
    def on_radioButton_BT_clicked(self, checked):
        if checked:
            self.solver.selected_algorithm = self.solver.backtracking
            self.update_step_iterator()

    @QtCore.pyqtSlot(bool)
    def on_radioButton_TS_clicked(self, checked):
        if checked:
            self.solver.selected_algorithm = self.solver.tabu_search
            self.update_step_iterator()

    @QtCore.pyqtSlot(bool)
    def on_radioButton_SA_clicked(self, checked):
        if checked:
            self.solver.selected_algorithm = self.solver.simulated_annealing
            self.update_step_iterator()

    @QtCore.pyqtSlot(bool)
    def on_radioButton_GA_clicked(self, checked):
        if checked:
            self.solver.selected_algorithm = self.solver.genetic_algorithm
            self.update_step_iterator()






if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MyApp()
    window.show()
    sys.exit(app.exec_())