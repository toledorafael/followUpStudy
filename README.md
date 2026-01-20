# Evaluation of Interface for Follow-up Queries
 
This repository includes the supporting material for the thesis chapter "Evaluation of Interface for Follow-up Queries."

## File description

The sections below explain the folders and files included in this repository.

### chess

This folder includes the C++ code implementing the chess game and the models extracted from that implementation using the fact extractor Rex. 

The subfolder `model` includes the file `chess.cypher`. The file contains queries used to create a factbase of the system in a [Neo4j database](neo4j.com). Queries create nodes representing code entities (e.g., variable, functions) and relationships between such entities (e.g., function calls, variable writes). The following steps describe the process to create the models adopted in our user study:
  1. Create a Neo4j database instance (you can [this tutorial](https://neo4j.com/docs/aura/auradb/getting-started/create-database/))
  2. Run queries in `chess.cypher`

### studyInterfaces
The files in this folder show the interfaces the participants interacted with during the study.

  * *ScreeningQuestionnaire.pdf*: Screenshots of the webpage used to test candidates. Students who correctly answered at leat 5 of the 6 questions were invited to participate in the study.
  * *studyTutorial.pdf*: Screenshots of the tutorial slides used to introduce participants to enhanced Neo4j Browser.
  * *studyTasks.pdf*: Screenshots of the webpage participants used to access and submit study questions and tasks. 

### rawData
This folder includes the data recorded during the study and used in statistical tests. Files used for quantitative analysis follow the name convention *variable.csv*, in which *variable* describes the dependent variable (correctness, efficiency, or cognitive load) being recorded. The folder also includes two spreadsheets containing participants demographics information (`demographics.xlsx`) and participants' response to the experience statements and open-ended feedback questions (`participantsFeedback.xlsx`). 

### statisticalTestResults
The results of the statistical tests using the some of the files from folder `rawData`. Files in this folder follow a similar variable name convention with the addition of suffix *_tTest* to describe the performed tests.
