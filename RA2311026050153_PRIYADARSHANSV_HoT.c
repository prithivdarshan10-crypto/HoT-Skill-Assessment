/*
 * HoT-Based Skill Assessment - Problem 9
 * DAG Optimization: Redundant Expression Detection
 * Tool: C
 * HOTS Goal: Detect optimization opportunities via common subexpression analysis
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NODES 20
#define MAX_BLOCKS 4
#define MAX_EXPR_LEN 10
#define MAX_BLOCK_LEN 5
#define REDUNDANCY_THRESHOLD 0.3

typedef struct {
    int    node_id;
    char   block_id[MAX_BLOCK_LEN];
    char   expression[MAX_EXPR_LEN];
    char   left_operand[5];
    char   right_operand[5];
    char   operator;
    int    expression_frequency;
    int    is_common_subexpression;
} DAGNode;

typedef struct {
    char   block_id[MAX_BLOCK_LEN];
    int    total_nodes;
    int    unique_expressions;
    int    redundant_expressions;
    int    max_expression_frequency;
    double redundancy_ratio;
    int    optimization_opportunity;
} BlockSummary;

/* Count how many times an expression appears in the same block */
int count_expression_in_block(DAGNode nodes[], int n, const char *block, const char *expr) {
    int count = 0;
    for (int i = 0; i < n; i++)
        if (strcmp(nodes[i].block_id, block) == 0 &&
            strcmp(nodes[i].expression, expr) == 0)
            count++;
    return count;
}

/* Count unique expressions in a block */
int count_unique_expressions(DAGNode nodes[], int n, const char *block) {
    char seen[MAX_NODES][MAX_EXPR_LEN];
    int  seen_count = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(nodes[i].block_id, block) != 0) continue;
        int found = 0;
        for (int j = 0; j < seen_count; j++)
            if (strcmp(seen[j], nodes[i].expression) == 0) { found = 1; break; }
        if (!found) strcpy(seen[seen_count++], nodes[i].expression);
    }
    return seen_count;
}

int main() {

    /* ── 1. Dataset ─────────────────────────────────────────── */
    DAGNode nodes[MAX_NODES] = {
        {1,  "B1", "a+b", "a","b",'+', 0, 0},
        {2,  "B1", "c*d", "c","d",'*', 0, 0},
        {3,  "B1", "a+b", "a","b",'+', 0, 0},
        {4,  "B1", "e-f", "e","f",'-', 0, 0},
        {5,  "B1", "c*d", "c","d",'*', 0, 0},
        {6,  "B2", "x/y", "x","y",'/', 0, 0},
        {7,  "B2", "p+q", "p","q",'+', 0, 0},
        {8,  "B2", "x/y", "x","y",'/', 0, 0},
        {9,  "B2", "x/y", "x","y",'/', 0, 0},
        {10, "B2", "r-s", "r","s",'-', 0, 0},
        {11, "B3", "m*n", "m","n",'*', 0, 0},
        {12, "B3", "a+b", "a","b",'+', 0, 0},
        {13, "B3", "u+v", "u","v",'+', 0, 0},
        {14, "B3", "m*n", "m","n",'*', 0, 0},
        {15, "B3", "w-z", "w","z",'-', 0, 0},
        {16, "B4", "t*s", "t","s",'*', 0, 0},
        {17, "B4", "a*b", "a","b",'*', 0, 0},
        {18, "B4", "c+d", "c","d",'+', 0, 0},
        {19, "B4", "e*f", "e","f",'*', 0, 0},
        {20, "B4", "a*b", "a","b",'*', 0, 0},
    };
    int n = MAX_NODES;

    /* ── 2. Compute Expression_Frequency & Is_Common_Subexpression ── */
    for (int i = 0; i < n; i++) {
        nodes[i].expression_frequency =
            count_expression_in_block(nodes, n, nodes[i].block_id, nodes[i].expression);
        nodes[i].is_common_subexpression = (nodes[i].expression_frequency > 1) ? 1 : 0;
    }

    /* ── 3. Print node-level analysis ───────────────────────── */
    printf("=================================================================\n");
    printf("DAG NODE ANALYSIS - Expression Frequency & CSE Detection\n");
    printf("=================================================================\n");
    printf("%-8s %-8s %-12s %-6s %-25s\n",
           "Node_ID","Block_ID","Expression","Freq","Is_Common_Subexpression");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-8d %-8s %-12s %-6d %-25s\n",
               nodes[i].node_id,
               nodes[i].block_id,
               nodes[i].expression,
               nodes[i].expression_frequency,
               nodes[i].is_common_subexpression ? "True" : "False");
    }

    /* ── 4. Block-level summary ──────────────────────────────── */
    char blocks[MAX_BLOCKS][MAX_BLOCK_LEN] = {"B1","B2","B3","B4"};
    BlockSummary summary[MAX_BLOCKS];

    for (int b = 0; b < MAX_BLOCKS; b++) {
        summary[b].total_nodes            = 0;
        summary[b].redundant_expressions  = 0;
        summary[b].max_expression_frequency = 0;
        strcpy(summary[b].block_id, blocks[b]);

        for (int i = 0; i < n; i++) {
            if (strcmp(nodes[i].block_id, blocks[b]) != 0) continue;
            summary[b].total_nodes++;
            if (nodes[i].is_common_subexpression)
                summary[b].redundant_expressions++;
            if (nodes[i].expression_frequency > summary[b].max_expression_frequency)
                summary[b].max_expression_frequency = nodes[i].expression_frequency;
        }

        summary[b].unique_expressions =
            count_unique_expressions(nodes, n, blocks[b]);

        summary[b].redundancy_ratio =
            (double)summary[b].redundant_expressions / summary[b].total_nodes;

        summary[b].optimization_opportunity =
            (summary[b].redundancy_ratio > REDUNDANCY_THRESHOLD) ? 1 : 0;
    }

    printf("\n=================================================================\n");
    printf("BLOCK-LEVEL SUMMARY\n");
    printf("=================================================================\n");
    printf("%-10s %-12s %-19s %-22s %-24s %-17s %-22s\n",
           "Block_ID","Total_Nodes","Unique_Expressions",
           "Redundant_Expressions","Max_Expr_Frequency",
           "Redundancy_Ratio","Optimization_Opportunity");
    printf("------------------------------------------------------------------"
           "-----------------------------------------------------------\n");
    for (int b = 0; b < MAX_BLOCKS; b++) {
        printf("%-10s %-12d %-19d %-22d %-24d %-17.4f %-22s\n",
               summary[b].block_id,
               summary[b].total_nodes,
               summary[b].unique_expressions,
               summary[b].redundant_expressions,
               summary[b].max_expression_frequency,
               summary[b].redundancy_ratio,
               summary[b].optimization_opportunity ? "True" : "False");
    }

    /* ── 5. Optimization candidates ─────────────────────────── */
    printf("\n=================================================================\n");
    printf("OPTIMIZATION CANDIDATES (Blocks with Redundancy Ratio > 0.3)\n");
    printf("=================================================================\n");
    printf("%-10s %-17s %-24s\n",
           "Block_ID","Redundancy_Ratio","Max_Expression_Frequency");
    printf("-------------------------------------------------\n");
    int found = 0;
    for (int b = 0; b < MAX_BLOCKS; b++) {
        if (summary[b].optimization_opportunity) {
            printf("%-10s %-17.4f %-24d\n",
                   summary[b].block_id,
                   summary[b].redundancy_ratio,
                   summary[b].max_expression_frequency);
            found = 1;
        }
    }
    if (!found) printf("No blocks exceed the redundancy threshold.\n");

    printf("\n[Done] Common subexpression analysis complete.\n");
    return 0;
}
