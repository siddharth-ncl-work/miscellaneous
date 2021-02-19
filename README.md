# miscellaneous and test repo
Contains useful stuff.
Created to keep other repositories clean
Can be used for testing purposes

* A successful Git branching model: https://nvie.com/posts/a-successful-git-branching-model/
* Git practice exercies: https://learngitbranching.js.org/?locale=en_US

## Modified Branching model for Science Project
* Long-Term Branches: Master, Develop, data-*-*, code-verification-*.*
* Short-Term Branches: release-*.*, hotfix-*.*
* Do not generate final data in a release branch. Create new data branch off of master/release to generate data for that particular version 
* Branch name : Use dash(-) instead of underscore 
* data-*-* : Only changes allowed on this branch are related to data storage and representation. Core code should not be changed at all
* code-verification-*.* : create necessary data which shows that code works for test inputs before releasing

