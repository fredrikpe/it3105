import sys
import threading

import time
from PyQt5 import QtWidgets, QtCore, uic

from algorithms.solver import Solver
from board import Board


class MyApp(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)

        # Using .ui files - from Qt Designer
        uic.loadUi('main_window.ui', self)

        self.board = Board(self)
        self.solver = Solver(self.board)

        self.main_grid_layout = self.gridLayout_2  # type: QtWidgets.QGridLayout
        self.main_grid_layout.addWidget(self.board, 2, 0, 1, 7)

        self.step_iterator = iter([])
        self.solutions_iterator = iter([])

        self.radioButton_BT.click()

    def update_step_iterator(self):
        self.on_queensLineEdit_textChanged(None)

    @QtCore.pyqtSlot()
    def on_solveButton_clicked(self):
        if len(self.solver.solutions) == 0:
            self.solveButton.setEnabled(False)
            self.solveButton.setText("Solving...")
            self.solver_thread = threading.Thread(target=self.start_solver, args=[])
            self.solver_thread.start()
        else:
            try:
                self.board.queens = next(self.solutions_iterator)
            except StopIteration:
                self.solutions_iterator = iter(self.solver.solutions)
                self.board.queens = next(self.solutions_iterator)
            self.board.update()

    def start_solver(self):
        start_time = time.time()
        board_size = self.board.board_size
        self.solver.find_solutions(self.board.queens)
        duration = time.time() - start_time

        # Parameters changed while solving, abort
        if board_size != self.board.board_size:
            self.solutions_iterator = iter([])
            self.solveButton.setEnabled(True)
            self.solveButton.setText("Find Solutions")
            return

        self.solutions_iterator = iter(self.solver.solutions)
        print("# of sols: ", len(self.solver.solutions))

        self.selected_label.setText(str(len(self.solver.solutions)) + "\n{:.2g} s".format(duration))

        self.solveButton.setEnabled(True)
        self.solveButton.setText("Next Solution")

    @QtCore.pyqtSlot(int)
    def on_boardSizeSpinBox_valueChanged(self, size):
        self.board.board_size = size
        self.on_queensLineEdit_textChanged(None)

    @QtCore.pyqtSlot(str)
    def on_queensLineEdit_textChanged(self, text):
        if not text:
            text = self.queensLineEdit.text()
        self.board.queens = [int(c) for c in text]
        self.solver.input_length = len(text)
        self.reset()

    def reset(self):
        self.solutions_iterator = iter([])
        self.solver.reset_solutions()
        self.solveButton.setText("Find Solutions")
        self.step_iterator = self.solver.step_solve(self.board.queens)
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
            self.selected_label = self.label_BT
            self.update_step_iterator()

    @QtCore.pyqtSlot(bool)
    def on_radioButton_TS_clicked(self, checked):
        if checked:
            self.solver.selected_algorithm = self.solver.tabu_search
            self.selected_label = self.label_TS
            self.update_step_iterator()

    @QtCore.pyqtSlot(bool)
    def on_radioButton_SA_clicked(self, checked):
        if checked:
            self.solver.selected_algorithm = self.solver.simulated_annealing
            self.selected_label = self.label_SA
            self.update_step_iterator()

    @QtCore.pyqtSlot(bool)
    def on_radioButton_GA_clicked(self, checked):
        if checked:
            self.solver.selected_algorithm = self.solver.genetic_algorithm
            self.selected_label = self.label_GA
            self.update_step_iterator()






if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MyApp()
    window.show()
    sys.exit(app.exec_())