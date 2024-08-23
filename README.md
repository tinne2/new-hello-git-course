# Git repo instructions

In the following folder structure, you can see 
- weekly exercises (wk*) and 
- the final assignment in two parts (prg1 and prg2):

|── prg1
│   ├── example-data
│   ├── functionality-compulsory
│   ├── functionality-optional
│   ├── integration-compulsory
│   ├── integration-optional
│   └── optimization_tests
├── prg2
│   ├── example-data
│   ├── functionality-compulsory
│   ├── functionality-optional
│   ├── integration-compulsory
│   └── integration-optional
├── wk02_decrease_or_divide
│   └── missing
├── wk03_stl
│   ├── iteration
│   └── stl
├── wk04_trees
│   ├── iterator_invalidation
│   └── valgrind
│       └── task_list_v2
└── wk05_graphs
    └── improving_functions



The weekly exercises have each exercise in a separate subfolder.  
The subfolders under the `prg1/2` folders contain the tests, such as `functionality-optional`, etc.  
You are supposed to run the tests before submitting to Plussa.

## Commit and Push Instructions

Commit and push your changes, then check from Course GitLab:

[https://course-gitlab.tuni.fi/compcs300-fall2024/USERNAME](https://course-gitlab.tuni.fi/compcs300-fall2024/USERNAME)

Ensure that your commit is visible before submitting to Plussa.

## Submission Guidelines

The submission is made using the Git URL and commit hash.

- **Git URL format:**

  ```
  git@course-gitlab.tuni.fi:compcs300-fall2024/USERNAME.git
  ```

- **To retrieve the commit hash:**
  
  You can copy it from the Course GitLab GUI or query it from the command line with the following command:

  ```
  git log
  ```

Take the **topmost hash** from the output.


## test.sh in prg1/2 as test utility

If you prefer working in command prompt, we provide you a script for easy and fast testing in prg1 and prg2 folders: 

  ```
  ./test.sh
  ```


It requires bash, and is targeted to be used in linux env. Linux env is available for every student
in linux-desktop, see:

[https://intra.tuni.fi/en/it-services/networks/linux-ssh-and-remote-desktop-servers](https://intra.tuni.fi/en/it-services/networks/linux-ssh-and-remote-desktop-servers)
