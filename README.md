# Evaluation of Interface for Follow-up Queries
 
This repository includes the supporting material for the thesis chapter "Evaluation of Interface for Follow-up Queries."

## File description

The sections below explain the folders and files included in this repository.

### chess

This folder includes the C++ code implementing the Graph Product Line [1,2] and the models extracted from that implementation using the fact extractor Rex<sup>&uarr;</sup> [3]. 

The subfolder `model` includes the files `nodes.cypher` and `edges.cypher`. Each file contain queries used to create a factbase of the system in a [Neo4j database](neo4j.com). Queries in `nodes.cypher` create nodes representing code entities (e.g., variable, functions) and `edges.cypher` include queries to create the relationships between such entities (e.g., function calls, variable writes). The following steps describe the process to create the models adopted in our user study:
  1. Create a Neo4j database instance (you can [this tutorial](https://neo4j.com/docs/aura/auradb/getting-started/create-database/))
  2. Run queries in `nodes.cypher`
  3. Run queries in `edges.cypher`

### studyInterfaces
The files in this folder show the interfaces the participants interacted with during the study.

  * *ScreeningQuestionnaire.pdf*: Screenshots of the webpage used to introduce the theme of the study and test candidates. Students whi correctly answered at leat 6 of the 7 questions were invited to participate in the study.
  * *stage1_(controlGroup|treatmentGroup).pdf*: Screenshots of the ^(Neo4j) Browser version used for the first stage of the study. Control participants did not have access the edge highlight feature.
  * *stage2_(controlGroup|treatmentGroup).pdf*: Screenshots of the webpage used during the second stage of the study. Participants in the treatment group were allocated in different subgroups that accessed different versions of this questionnaire, each version would have an distinct sequence of visualization options.

### rawData
This folder includes the data recorded during the study and used in statistical tests. Files used for quantitative analysis follow the name convention *scenario_variable.csv*, in which *scenario* describes the task category (identify, compare2, or compare3), the study stage (stage1 or stage 2), or the treatment type (visualizationOption) being tested, and *variable* describes the dependent variable (correctness, efficiency, or mental effort) being recorded. The folder also includes two spreadsheets containing participants demographics information (`demographics.xlsx`) and participants' response to the experience statements and open-ended feedback questions (`participantsFeedback.xlsx`). 

### statisticalTestResults
The results of the statistical tests using the files from folder `rawData`. Files in this folder follow a similar name convention with the addition of suffixes *_tTest* and *_OneWay Anova* to describe the performed tests.
