
#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "montador.h"

Test(montador_suite, test_CREATE_PROGRAM) {

    FILE *input = fopen("test_create_program", "w");
    assert(input != NULL );
    char input_test[] = 
        " omasasoasdasdmo domsod \n"
        " ndsainasksndsjs !! !!!\n"
        " LOAD LOADL OAMDO SM \n";

    fprintf(input, "%s", input_test);
    fclose(input);
    input = fopen("test_create_program", "r");
    program_t *program = createProgram(input);    
    fclose(input);

    cr_assert_str_eq( input_test, program->source );
    cr_assert_eq( 0, program->HEAD );
    cr_assert_eq( NULL, program->sections );
    cr_assert_eq( NULL, program->table );
    cr_assert_eq( strlen(input_test), program->program_size );
    cr_assert_eq( NULL, program->globals );
    cr_assert_eq( NULL, program->externs );

    freeProgram(program);
}

Test(montador_suite, test_NEXT_TOKEN)
{

    FILE *input = fopen("test_next_token", "w");
    assert(input != NULL );
    char source[] = 
    " &LOAD 20 STORE 29";

    fprintf(input, "%s", source);
    fclose(input);
    input = fopen("test_next_token", "r");
    program_t *program = createProgram(input);    
    fclose(input);

    token_t *tokens =
    (token_t *) malloc( 3 * sizeof ( token_t ) );

    tokens[0].token = "&";
    tokens[0].defined = true;
    tokens[0].value  = -1;
    tokens[0].type   = TOK_ADDRESSING;

    tokens[1].token = "LOAD";
    tokens[1].defined = false;
    tokens[1].value  = -1;
    tokens[1].type   = TOK_LOAD;

    tokens[2].token = "20";
    tokens[2].defined = true;
    tokens[2].value  = 20;
    tokens[2].type   = TOK_LITERAL;

    token_t *program_tokens = 
        (token_t *) malloc ( 3 * sizeof( token_t ) );

    program->tokens = program_tokens;
    token_t tok = nextToken(program);
    program_tokens[0] = tok;

    cr_assert_eq( program->HEAD, 2 );
    cr_assert ( program->tokens[0].defined == tokens[0].defined );
    cr_assert ( program->tokens[0].value == tokens[0].value );
    cr_assert ( program->tokens[0].type == tokens[0].type );
    cr_assert_str_eq( program->tokens[0].token, tokens[0].token);

    tok = nextToken(program);
    program_tokens[1] = tok;

    cr_assert_str_eq( program->tokens[1].token, tokens[1].token); 
    cr_assert ( program->tokens[1].defined == tokens[1].defined );
    cr_assert ( program->tokens[1].value == tokens[1].value );
    cr_assert ( program->tokens[1].type == tokens[1].type );
    cr_assert_eq( program->HEAD, 6 );

    tok = nextToken(program);
    program_tokens[2] = tok;

    cr_assert_str_eq( program->tokens[2].token, tokens[2].token); 
    cr_assert ( program->tokens[2].defined == tokens[2].defined );
    cr_assert ( program->tokens[2].value == tokens[2].value );
    cr_assert ( program->tokens[2].type == tokens[2].type );
    cr_assert_eq( program->HEAD, 9 );

}

Test(montador_suite, test_PARSE) 
{

    FILE *input = fopen("test_parse", "w");
    assert(input != NULL );
    char source[] = 
    " LOAD 20 STORE &29\n";

    fprintf(input, "%s", source);
    fclose(input);
    input = fopen("test_parse", "r");
    program_t *program = createProgram(input);    
    fclose(input);

    tokenize(program);
    parse(program);

    cr_assert_eq( program->sections->dot_text->array[0], LOAD_IMMEDIATE );
    cr_assert_eq( program->sections->dot_text->array[1], 20 );
    cr_assert_eq( program->sections->dot_text->array[2], STORE_DIRECT   );
    cr_assert_eq( program->sections->dot_text->array[3], 29 );

    freeProgram(program);
}

Test(montador_suite, test_TOKENIZE) 
{
    FILE *input = fopen("test_tokenize", "w");
    assert(input != NULL );
    char source[] = 
    " &LOAD 20 STORE 29";

    fprintf(input, "%s", source);
    fclose(input);
    input = fopen("test_tokenize", "r");
    program_t *program = createProgram(input);    
    fclose(input);

    token_t *exp_tokens =
        ( token_t * ) malloc ( 5 * sizeof(token_t) );

    exp_tokens[0].token = "&";
    exp_tokens[1].token = "LOAD";
    exp_tokens[2].token = "20";
    exp_tokens[3].token = "STORE";
    exp_tokens[4].token = "29";

    exp_tokens[0].type  = TOK_ADDRESSING;
    exp_tokens[1].type  = TOK_LOAD;
    exp_tokens[2].type  = TOK_LITERAL;
    exp_tokens[3].type  = TOK_STORE;
    exp_tokens[4].type  = TOK_LITERAL;

    tokenize(program);

    for ( int i=0; i < 5; i++ ) {
        cr_assert_str_eq( program->tokens[i].token, exp_tokens[i].token );
        cr_assert_eq( program->tokens[i].type, exp_tokens[i].type);
    }

    freeProgram(program);
}

Test(montador_suite, test_APPEND_SECTIONS) {}

Test(montador_suite, test_INTERFACE) {}

//Test(montador_suite, test_ASSEMBLE_PROGRAM) {
//   
//    FILE *input = fopen("some_path", "r");
//    assert(input != NULL);
//    program_t *program = createProgram(input);
//    tokenize(program);
//    symbol_table_t *table = parse(program);
//    FILE *output = fopen("some_path_2", "w");
//    assert(output != NULL);
//    appendSections( program, output );   
//
//}

