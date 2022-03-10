# snippet

Light-weight note tracking app in the command line ... for when you're journaling on-the-go.

## Features

Existing features:

- [x] Save a new note specific to a project: `snippet -p "My project" -a "Note to self"`
- [x] List all notes for a project: `snippet -p "My project"`
- [x] Check projects with active notes: `snippet --project-list`
- [x] Edit a previous note from the command line: `snippet -e <entry_id>`

Features in the works:

- [ ] List recent notes
- [ ] Maintain a configuration file (e.g., YAML)
- [ ] Delete entry

## Installation

Clone the repo and run `Make` after changing into the main directory. Run the program by referencing the `snippet` executable. If desired, you can add this folder to your `PATH` variable to call snippet from any directory. Alternatives include creating a Bash alias.
