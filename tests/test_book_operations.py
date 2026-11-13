"""Run against a compiled application, using temporary inventory files only.

Usage: python tests/test_book_operations.py path/to/library.exe
"""

import pathlib
import subprocess
import sys
import tempfile
import unittest


APPLICATION = pathlib.Path(sys.argv.pop(1)).resolve()
ORIGINAL = '"B1" "The Hobbit" "J. R. R. Tolkien" 1937 5\n'


class BookOperationTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory(prefix="library-operations-")
        self.addCleanup(self.folder.cleanup)
        self.data = pathlib.Path(self.folder.name, "library_data.txt")
        self.data.write_text(ORIGINAL, encoding="utf-8")

    def run_app(self, entries):
        result = subprocess.run(
            [str(APPLICATION)], input="\n".join(entries) + "\n",
            text=True, capture_output=True, cwd=self.folder.name, timeout=5,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stderr, "")
        return result.stdout

    def assert_inventory(self, expected):
        self.assertEqual(self.data.read_text(encoding="utf-8"), expected)

    def test_empty_update_and_remove_skip_id_prompt(self):
        self.data.write_text("", encoding="utf-8")
        output = self.run_app(["4", "5", "6", "1"])
        self.assertEqual(output.count("The library inventory is empty."), 2)
        self.assertNotIn("Enter book ID:", output)
        self.assert_inventory("")

    def test_each_field_update_preserves_other_fields(self):
        cases = [
            ("1", "Revised Title", '"B1" "Revised Title" "J. R. R. Tolkien" 1937 5\n'),
            ("2", "O'Connor", '"B1" "The Hobbit" "O\'Connor" 1937 5\n'),
            ("3", "1450", '"B1" "The Hobbit" "J. R. R. Tolkien" 1450 5\n'),
            ("4", "1", '"B1" "The Hobbit" "J. R. R. Tolkien" 1937 1\n'),
        ]
        for field, value, expected in cases:
            with self.subTest(field=field):
                self.data.write_text(ORIGINAL, encoding="utf-8")
                output = self.run_app(["4", "B1", field, value, "y", "6", "1"])
                self.assertIn("Book updated successfully.", output)
                self.assert_inventory(expected)

    def test_update_cancel_and_decline_preserve_record(self):
        for entries in (["4", "B1", "5", "6", "1"],
                        ["4", "B1", "1", "Changed", "n", "6", "1"]):
            with self.subTest(entries=entries):
                self.assertIn("Update cancelled.", self.run_app(entries))
                self.assert_inventory(ORIGINAL)

    def test_invalid_submenu_year_and_upper_year_boundary(self):
        output = self.run_app(["4", "B1", "letters", "6", "3", "1449", "2027", "2026", "y", "6", "1"])
        self.assertIn("Enter a whole number from 1 to 5.", output)
        self.assertIn("Enter a whole number from 1450 to 2026.", output)
        self.assert_inventory(ORIGINAL.replace("1937", "2026"))

    def test_blank_title_and_invalid_copies_are_reprompted(self):
        output = self.run_app(["4", "B1", "1", "   ", "The Hobbit", "y",
                               "4", "B1", "4", "letters", "-2", "-1", "5", "y", "6", "1"])
        self.assertIn("This field cannot be blank.", output)
        self.assertIn("Enter a whole number from 1 to 2147483647.", output)
        self.assert_inventory(ORIGINAL)

    def test_remove_some_copies_and_reload(self):
        output = self.run_app(["5", "B1", "1", "2", "y", "6", "1"])
        self.assertIn("Copies removed successfully. Remaining copies: 3", output)
        self.assert_inventory(ORIGINAL.replace("1937 5", "1937 3"))
        output = self.run_app(["2", "6", "1"])
        self.assertIn("The Hobbit", output)
        self.assertIn("Total book records: 1", output)

    def test_remove_invalid_quantities_then_one_copy(self):
        output = self.run_app(["5", "B1", "1", "letters", "-2", "-1", "6", "1", "yes", "6", "1"])
        self.assertEqual(output.count("Enter a whole number from 1 to 5."), 4)
        self.assert_inventory(ORIGINAL.replace("1937 5", "1937 4"))

    def test_remove_all_via_quantity_or_record_option(self):
        for entries in (["5", "B1", "1", "5", "y", "6", "1"],
                        ["5", "B1", "2", "yes", "6", "1"]):
            with self.subTest(entries=entries):
                self.data.write_text(ORIGINAL, encoding="utf-8")
                output = self.run_app(entries)
                self.assertIn("This will delete the entire book record.", output)
                self.assertIn("Book removed successfully.", output)
                self.assert_inventory("")

    def test_removal_cancel_and_decline_preserve_record(self):
        for entries in (["5", "B1", "3", "6", "1"],
                        ["5", "B1", "1", "2", "n", "6", "1"],
                        ["5", "B1", "2", "no", "6", "1"]):
            with self.subTest(entries=entries):
                self.assertIn("Book removal cancelled.", self.run_app(entries))
                self.assert_inventory(ORIGINAL)

    def test_missing_ids_do_not_modify_inventory(self):
        output = self.run_app(["4", "missing", "5", "missing", "6", "1"])
        self.assertEqual(output.count("Book ID not found."), 2)
        self.assert_inventory(ORIGINAL)

    def test_confirmation_reprompts(self):
        output = self.run_app(["5", "B1", "1", "2", "maybe", "n", "6", "1"])
        self.assertIn("Please enter Y or N.", output)
        self.assert_inventory(ORIGINAL)

    def test_eof_does_not_hang_or_apply_partial_changes(self):
        for entries in (["4"], ["4", "B1"], ["4", "B1", "1"],
                        ["4", "B1", "1", "Changed"], ["5"],
                        ["5", "B1"], ["5", "B1", "1"],
                        ["5", "B1", "1", "2"]):
            with self.subTest(entries=entries):
                output = self.run_app(entries)
                self.assertIn("Input closed. Exiting program...", output)
                self.assert_inventory(ORIGINAL)

    def test_add_duplicate_id_search_and_author_punctuation(self):
        output = self.run_app(["1", "b1", "B2", "New Book", "O'Connor", "2000", "2",
                               "3", "hOBb", "2", "6", "1"])
        self.assertIn("Book ID already exists.", output)
        self.assertIn("Search results:", output)
        self.assertIn("The Hobbit", output)
        self.assertIn("Total book records: 2", output)
        self.assert_inventory(ORIGINAL + '"B2" "New Book" "O\'Connor" 2000 2\n')

    def test_actions_open_without_continue_screen(self):
        output = self.run_app(["1", "0", "2", "3", "0", "4", "0", "5", "0", "6", "0", "6", "1"])
        self.assertNotIn("Continue or cancel:", output)
        self.assertNotIn("1. Continue", output)
        self.assertNotIn("/cancel", output)
        self.assertIn("(press 0 to cancel)", output)
        self.assertEqual(output.count("Action cancelled. Returning to the main menu."), 5)
        self.assertEqual(output.count("Data saved. Exiting program..."), 1)
        self.assert_inventory(ORIGINAL)

    def test_cancel_at_each_add_prompt_discards_partial_record(self):
        values = ["B2", "New Title", "New Author", "2000", "3"]
        for position in range(len(values)):
            with self.subTest(position=position):
                output = self.run_app(["1"] + values[:position] + ["0", "6", "1"])
                self.assertIn("Action cancelled. Returning to the main menu.", output)
                self.assertNotIn("Book added successfully.", output)
                self.assert_inventory(ORIGINAL)

    def test_cancel_during_search_update_and_remove(self):
        scripts = [
            ["3", "0"],
            ["4", "0"],
            ["4", "B1", "0"],
            ["4", "B1", "1", "0"],
            ["4", "B1", "4", "0"],
            ["4", "B1", "1", "Changed", "0"],
            ["5", "0"],
            ["5", "B1", "0"],
            ["5", "B1", "1", "0"],
            ["5", "B1", "1", "2", "0"],
        ]
        for script in scripts:
            with self.subTest(script=script):
                output = self.run_app(script + ["6", "1"])
                self.assertIn("Action cancelled. Returning to the main menu.", output)
                self.assert_inventory(ORIGINAL)

    def test_zero_cancel_accepts_surrounding_whitespace(self):
        for entries in (["1", " 0 ", "6", "1"],
                        ["4", "B1", "4", " 0 ", "6", "1"],
                        ["6", " 0 ", "6", "1"]):
            with self.subTest(entries=entries):
                output = self.run_app(entries)
                self.assertIn("Action cancelled. Returning to the main menu.", output)
                self.assert_inventory(ORIGINAL)


if __name__ == "__main__":
    unittest.main(verbosity=2)
