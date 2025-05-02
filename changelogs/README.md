# OPOSSUM Change Log
All notable changes to OPOSSUM will be documented in the file: 
[`changelog.md`](changelog.md).

If additional documentation is required, make a file in: 
[`/changelog_details`](changelog_details/). <br>
 Ensure the file name is the same as the version number. <br> 
Example: [`/changelog_details/README.md`](changelog_details/README.md)

## Markdown references from [markdownguide.org](https://www.markdownguide.org/)
- [Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)
- [Basic syntax guide](https://www.markdownguide.org/extended-syntax/)
- [Extended syntax guide](https://www.markdownguide.org/extended-syntax/)

`<!-->` This is a comment which is not shown in the "view" mode. <br>
Please use `<!-->` between versions to make it easier when reading/editing the markdown.

<!------------------------------------------------------------>
# Change Log Formatting Guide
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

### Version Numbering
| Number | Change Description            | Effects                     |
| ------ | :---------------------------: | --------------------------- |
| MAJOR  | Incompatible changes          | Resets MINOR and MAJOR to 0 |
| MINOR  | Backward compatible changes   | Resets PATCH to 0           |
| PATCH  | Backward compatible bug fixes | Does not reset numbers      |

### Sub-headings
- [Added] - new features.
- [Changed] - changes in existing functionality.
- [Deprecated] - soon-to-be removed features.
- [Removed] - now removed features.
- [Fixed] - any bug fixes.
- [Security] - in case of vulnerabilities.

<!------------------------------------------------------------>
## Change Log Formatting Example

## `MAJOR.MINOR.PATCH` dd-MMM-yyyy
Here we write upgrading notes. Try to make them as straightforward as possible. <br>
Remember to use [`/changelog_details`](changelog_details/) if more detailed documentation is needed.

See details: [MAJOR.MINOR.PATCH](changelog_details/README.md)

### MAJOR
- [Added] - Any incompatible changes

### MINOR
- [Added] - Any backward compatible changes or major bug fixes

### PATCH
- [Added] - Any backward compatible bug fixes or small changes

<!------------------------------------------------------------>
## `3.0.0` 12-MAR-2025

### MAJOR
- [Fixed] - Module foo's timeline uses the browser timezone for date resolution

### MINOR
- [Changed] - Drupal.org is now used for composer.

### PATCH
- [Added] - New logic to run sheet teaser delete to delete corresponding schedule cards.

<!------------------------------------------------------------>
## `1.3.0` 09-MAR-2025

### MAJOR

### MINOR
- [Fixed] - module bar tests

### PATCH

<!------------------------------------------------------------>
## `1.2.3` 09-MAR-2025

### MAJOR

### MINOR
- [Fixed] - module foo tests

### PATCH
<!------------------------------------------------------------>
