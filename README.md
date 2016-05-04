# lhcvmm
Large Hadron Collider Visual Music Machine

[![Join the chat at https://gitter.im/Opensemble](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Opensemble)

# What lives here

* rendererExample: this is the very first inner-project of LHCVMM. Is a video renderer engine built on Open Frameworks needed to test drive the Full-Dome hardware where the final project will be presented.


# Collaborating

We are using, for the time being, a centralized git workflow and a single branch `master`. Hopefully we'll grow enough to embrace a more complex git workflow.

### starting

First of all, you'll need to clone this repo. We'll do it on the command line as is the easiest way to *understand* Git.

```bash

git clone git@github.com:Opensemble/lhcvmm.git

```

You'll have now a directory named lhcvmm with the source code inside.


### Adding and publishing changes

Once cloned, you can start working on your working copy, add new files or changes.
In order to share them withthe community you'll need to upload them to Github again.
This is done in 3 steps:

1. Add you changes
2. get other people's changes
3. upload to github

#### 1. Add you changes
 After modifiying the files you need to add the changes to version control:

```bash

git add <filename>

```

where `<filename>` can be a single file, a list of files separated by spaces, a directory (this will add every file changed under that directory and its children)


Then you need to bundle those changes in a commit:

```bash

git commit

```

This will open a text editor so you can leave a message so other developers in the same project can know what changes you made, why you made them, and any other information you think it will be useful or necesary for other people. Think of this message as a mail that you would send to another developer/artist regarding what you've worked on. (see the section `commit messages format` for a detailed explanation how to write these)
This may look as a boring o boureaucratic step, but in open source projects, a good communication is a key piece of a successful project, and having a readable commit history is part of that.


#### 2. Get Other peoples changes

If while you were working other people pushed their commits to github, then Git will ask you to update your working copy, as there are changes already published that you don't have.
And to avoid problems, you always have to be up to date before publishing your changes. For this you do:

```bash

git pull --rebase

```

This is command will connect to Github, fetch the last changes, and add all of those before the changes you commited in the previous step.
(pull will fetch the changes, and the --rebase option will place them before your work).

If the changes you brought from Github are on different files and or different lines, then your local repository is now updated.
But what happens if in the changes you pulled from Github, another developer changed exactly the same lines you also changed?
How does Git know which of the two versions is the correct?

*It does not!*

That's called a *conflict* and Git will let you know that it encountered one while merging your changes with the other's people changes.
And you have to resolve it by hand (this is one of the things the commits messages are usefull for :) )
You'll see something like:

```Git

On branch branch-b
 # You have unmerged paths.
 #   (fix conflicts and run "git commit")
 #
 # Unmerged paths:
 #   (use "git add ..." to mark resolution)
 #
 # both modified:      planets.md
 #
 no changes added to commit (use "git add" and/or "git commit -a")

```

To resolve a conflict you'll need to edit the file with conflicts (_planets.md_ in this example) and search for the lines between:

```diff

<<<<<<< HEAD (your current version)
nine
=======
eight
>>>>>>> another-commit-hash (the version published by the other dev)

```

So you need to decide which of the two set of lines is the correct one, the one before the `=====` or the one after those.
Once decided, you erase the other one, and all the weird chars added by Git.
Next you need to tell git that you resolved the conflict:

```bash

git add <the-file-or-files-with-resolved-conflicts>

```

And to continue with the update process you run:

```bash

git rebase --continue

```

This command will stop again if it finds another conflict. You'll need to repeat the process.

If it doesn't find more conflicts, then, you are ready to upload your changes to Github


#### 3. upload to github

To send you changes to github you run:

```bash

git push

```




### Apendix 1: Commit Message Format

Here are some pretty good guidelines and arguments about good commit messages

https://robots.thoughtbot.com/5-useful-tips-for-a-better-commit-message

And here about how should be formatted:

http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html


### Apendix 2: The ultimate Git documentation

https://git-scm.com/book/en/v2

chapters 2 and 3 should be more than enough to begin understandig Git :)
