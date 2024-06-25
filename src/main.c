#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <spm/libspm.h>
#include <spm/cutils.h>
#include <spm/hashtable.h>
#include <spm/globals.h>

#include "raylib.h"

#define SECTION_EXPLORE 0
#define SECTION_INSTALLED 1
#define SECTION_UPDATES 2
#define MAX_INPUT 48

// Globals
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// Functions
int GetCurrentSection();

// Drawing 
void DrawSectionExplore();
void DrawSectionInstalled();
void DrawSectionUpdates();

void DrawSearchBar();
void DrawList(char** results, int num_results, char* split, Vector2 mouses, Font font);

int main(int argc, char *argv[]) 
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "COLPAK");
    init();

    //TODO: Create a hasmap of all the packages:
    //      List packages from the hasmap, automatically
    //      fetching the version, repo and wheter it's
    //      installed.

    //TODO: Implement the installation of packages:
    //      package page, console output, etc..
        
    SetWindowMinSize(800, 600);
    SetTargetFPS(60);

    Color bg = {23, 23, 23, 255};
    int section = SECTION_EXPLORE;
    Vector2 mouse = {0.0f, 0.0f};

    Font mono = LoadFontEx("resources/FreeMonoBold.ttf", 32, 0, 250);

    char** results = NULL;
    int num_results = 0;

    char* search_term = calloc(MAX_INPUT, 1);
    sprintf(search_term, "Search for...");
    int search_size = 0;

    while (!WindowShouldClose())
    {
        SCREEN_WIDTH = GetScreenWidth();
        SCREEN_HEIGHT = GetScreenHeight();
        
        BeginDrawing();

            ClearBackground(bg);

            mouse = GetMousePosition();
            int section_last = section;
            section = GetCurrentSection(section, mouse, mono);
            if(section != section_last)
            {
                free(search_term);
                search_term = calloc(MAX_INPUT, 1);
                sprintf(search_term, "Search for...");
                search_size = 0;
            }
            
            switch(section)
            {
                case SECTION_EXPLORE:
                    DrawSectionExplore(&results, &num_results, search_term, &search_size, mouse, mono);
                    break;
                case SECTION_INSTALLED: 
                    DrawSectionInstalled(search_term, &search_size, mouse, mono);
                    break;
                case SECTION_UPDATES:
                    DrawSectionUpdates(search_term, &search_size, mouse, mono);
                    break;
            }

        EndDrawing();
    }

    UnloadFont(mono);
    CloseWindow();

    return 0;
}

int GetCurrentSection(int section, Vector2 mouse, Font font)
{
    Color fg = {23, 23, 23, 255};

    Rectangle explore = {(SCREEN_WIDTH/2.0f - (SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f * 3)) + ((SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f) * SECTION_EXPLORE), SCREEN_HEIGHT/32.0f, SCREEN_WIDTH/8.0f,  SCREEN_HEIGHT/16.0f};
    Rectangle installed = {(SCREEN_WIDTH/2.0f - (SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f * 3)) + ((SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f) * SECTION_INSTALLED), SCREEN_HEIGHT/32.0f, SCREEN_WIDTH/8.0f,  SCREEN_HEIGHT/16.0f};
    Rectangle updates = {(SCREEN_WIDTH/2.0f - (SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f * 3)) + ((SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f) * SECTION_UPDATES), SCREEN_HEIGHT/32.0f, SCREEN_WIDTH/8.0f,  SCREEN_HEIGHT/16.0f};

    DrawRectangleRounded(explore, 0.5, 3, fg);
        DrawTextEx(font, "Explore", (Vector2){explore.x + explore.width/2.0f - strlen("Explore")*20/4.0f, explore.y + explore.height/2.0f - 8}, 16, 1, RAYWHITE);
    DrawRectangleRounded(installed, 0.5, 3, fg);
        DrawTextEx(font, "Installed", (Vector2){installed.x + installed.width/2.0f  - strlen("Installed")*20/4.0f, installed.y + installed.height/2.0f - 8}, 16, 1, RAYWHITE);
    DrawRectangleRounded(updates, 0.5, 3, fg);
        DrawTextEx(font, "Updates", (Vector2){updates.x + updates.width/2.0f - strlen("Updates")*20/4.0f, updates.y + updates.height/2.0f - 8}, 16, 1, RAYWHITE);

    if(CheckCollisionPointRec(mouse, explore))
    {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            return SECTION_EXPLORE;
        }
    }
    if(CheckCollisionPointRec(mouse, installed))
    {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            return SECTION_INSTALLED;
        }
    }
    if(CheckCollisionPointRec(mouse, updates))
    {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            return SECTION_UPDATES;
        }
    }
    return section;
}

void DrawSectionExplore(char*** results, int* num_results, char* search_term, int* search_size, Vector2 mouse, Font font)
{   
    char* temp_term = strdup(search_term);

    DrawSearchBar(search_term, search_size, mouse, font);

    if(strcmp(temp_term, search_term) != 0)
    {
        if(*results != NULL)
        {
            for(int i = 0; i < *num_results; i++)
            {
                printf("HERE '%s' '%d'  '%d' \n", (*results)[i], i, *num_results);
                free((*results)[i]);
                printf("NOW HERE \n");
            }
            free(*results);
            *results = NULL;
            *num_results = 0;
        }
        printf("ESCAPED \n");
        *results = search(search_term, num_results);
    }
    
    free(temp_term);

    Color fg = {43, 43, 43, 255};
    Rectangle explore = {(SCREEN_WIDTH/2.0f - (SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f * 3)) + ((SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f) * SECTION_EXPLORE), SCREEN_HEIGHT/32.0f, SCREEN_WIDTH/8.0f,  SCREEN_HEIGHT/16.0f};

    DrawRectangleRounded(explore, 0.5, 3, fg);
        DrawTextEx(font, "Explore", (Vector2){explore.x + explore.width/2.0f - strlen("Explore")*20/4.0f, explore.y + explore.height/2.0f - 8}, 16, 1, RAYWHITE);

    DrawList(*results, *num_results, ">", mouse, font);
}

void DrawSectionInstalled(char* search_term, int* search_size,  Vector2 mouse, Font font)
{
    int num;
    char** results = getAllFiles(getenv("SOVIET_SPM_DIR"), getenv("SOVIET_SPM_DIR"), &num);

    Color fg = {43, 43, 43, 255};
    Rectangle installed = {(SCREEN_WIDTH/2.0f - (SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f * 3)) + ((SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f) * SECTION_INSTALLED), SCREEN_HEIGHT/32.0f, SCREEN_WIDTH/8.0f,  SCREEN_HEIGHT/16.0f};

    DrawRectangleRounded(installed, 0.5, 3, fg);
        DrawTextEx(font, "Installed", (Vector2){installed.x + installed.width/2.0f  - strlen("Installed")*20/4.0f, installed.y + installed.height/2.0f - 8}, 16, 1, RAYWHITE);
           
    DrawList(results, num, "/", mouse, font); 
    free(results);
}

void DrawSectionUpdates(char* search_term, int* search_size,  Vector2 mouse, Font font)
{
    Color fg = {43, 43, 43, 255};
    Rectangle updates = {(SCREEN_WIDTH/2.0f - (SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f * 3)) + ((SCREEN_WIDTH/8.0f + SCREEN_WIDTH/32.0f) * SECTION_UPDATES), SCREEN_HEIGHT/32.0f, SCREEN_WIDTH/8.0f,  SCREEN_HEIGHT/16.0f};
    
    DrawRectangleRounded(updates, 0.5, 3, fg);
        DrawTextEx(font, "Updates", (Vector2){updates.x + updates.width/2.0f - strlen("Updates")*20/4.0f, updates.y + updates.height/2.0f - 8}, 16, 1, RAYWHITE);
}

void DrawSearchBar(char* search_term, int* search_size, Vector2 mouse, Font font)
{
    Color fg = {43, 43, 43, 255};
    Rectangle search = {SCREEN_WIDTH/2.0f - SCREEN_WIDTH/3.0f, SCREEN_HEIGHT/32.0f + SCREEN_HEIGHT/10.0f, SCREEN_WIDTH/2.0f + SCREEN_WIDTH/6.0f,  SCREEN_HEIGHT/16.0f};

    if(CheckCollisionPointRec(mouse, search))
    {
        DrawRectangleRounded(search, 0.5, 3, fg);
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    
        int key = GetCharPressed();
    
        while(key > 0) 
        { 
            if((key >= 32) && (key<= 125) && (*search_size < MAX_INPUT)) 
            { 
                search_term[*search_size] = (char)key; 
                search_term[*search_size + 1] = '\0'; 
                *search_size += 1; 
            }

            key = GetCharPressed();
        }
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            *search_size -= 1;
            if(*search_size < 0){*search_size = 0;}
            search_term[*search_size] = '\0';
        } 
        if(IsKeyPressedRepeat(KEY_BACKSPACE))
        {
            if(IsKeyDown(KEY_BACKSPACE))
            {
                *search_size -= 1;
                if(*search_size < 0){*search_size = 0;}
                search_term[*search_size] = '\0';
            }
        }
    }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

    DrawRectangleRoundedLinesEx(search, 0.5, 3, 3, fg);
    DrawTextEx(font, search_term, (Vector2){search.x + 20.0f, search.y + search.height/2.0f - 8}, 16, 1, RAYWHITE);
}

void DrawList(char** results, int num_results, char* split, Vector2 mouses, Font font)
{
    if(results != NULL)
    {
        int j = 0;
        for (int i = 0; i < num_results; i++)
        {
            char* temp = strdup(results[i]);
            Color fg = {43, 43, 43, 255};



            char* repo = strtok(temp, split);

            char* pkg = strchr(temp, '\0') + 1;
            char* temp_pkg = strdup(pkg);
            
            if(strlen(temp_pkg) > 16)
            {
                temp_pkg = realloc(temp_pkg, 18);
                temp_pkg[16] = '\0';
                sprintf(temp_pkg, "%s...", temp_pkg);
            }
            
            if(strcmp(repo, ".git") != 0)
            {
                j++;
                Rectangle result = {SCREEN_WIDTH/2.0f - SCREEN_WIDTH/3.0f, SCREEN_HEIGHT/4.0f + (34.0f*j), SCREEN_WIDTH/2.0f + SCREEN_WIDTH/6.0f, 32.0f};
                DrawRectangleRounded(result, 0.5, 3, fg);
                DrawTextEx(font, temp_pkg, (Vector2){result.x + 40.0f, result.y + result.height/2.0f - 8}, 16, 1, RAYWHITE);
                DrawTextEx(font, repo, (Vector2){result.x + result.width - 180.0f, result.y + result.height/2.0f - 8}, 16, 1, RAYWHITE);
            }
            free(temp_pkg);
            free(temp);
        }
    }
}













/*
    // install from source function
    int _install_source_(unsigned int* i) {
        exit(install_package_source(ARGV[++(*i)],0));
    }
    // remove a pkg function
    int _remove_(unsigned int* i) {
        char* pkg = ARGV[++(*i)];
        msg(INFO, "removing %s", pkg); 
        exit(uninstall(pkg));
    }
    // install from repo function 
    int _install_repo_(unsigned int* i) {
        struct package* pkg = calloc(1, sizeof(struct package));
        pkg->name = ARGV[++(*i)];

        char* pkg_name = calloc(strlen(pkg->name) + strlen(getenv("SOVIET_DEFAULT_FORMAT")) + 2, sizeof(char));
        if(!strstr(pkg->name, ".ecmp"))
        {
            sprintf(pkg_name, "%s.%s", pkg->name, getenv("SOVIET_DEFAULT_FORMAT"));
        }
            else
            {
                pkg_name = strdup(pkg->name);
            }

        int num_results;
        char** results = search(pkg_name, &num_results);

        char* repo;

        if(results != NULL)
        {
            for ( int i = 0; i < num_results; i++)
            {
                // Package name
                char* temp_1 = strtok(results[i], ">");
                // Repo it's in
                char* temp_2 = strchr(results[i], '\0') + 1;

                if(strcmp(getenv("SOVIET_DEFAULT_REPO"), temp_2) == 0)
                {
                    repo = temp_2;
                    break;
                }
                    else if (i == num_results) 
                    {
                        repo = temp_2;
                    }
            }
        }
        
        if (repo == NULL) {
            msg(ERROR, "Failed to download package %s", pkg->name);
            return 1;
        }

        get(pkg, repo, pkg->name);

        ask_to_preview_pkg(pkg->name);

        // TODO:
        // Accept a --opt "opt" argument then 
        // Check if a dependency is in the opt string

        // Attempt to open the package archive
        if (open_pkg(pkg->name, pkg, getenv("SOVIET_DEFAULT_FORMAT")) != 0) {
            msg(ERROR, "Failed to open package");
            return -1;
        }

        dbg(1, "Checking optional dependencies...");


        // Checking optional dependencies
        if (pkg->optionalCount > 0) {
            dbg(1, "Checking optional dependencies...");
            check_optional_dependencies(pkg->optional, pkg->optionalCount);
        }

        // TODO:
        // Accept a --in "in or --in def argument 
        // Then check if the number of arguments in
        // The string is equal to number of inputs
        // If so, supply the inputs NQA
        dbg(1, "Handling inputs...");

        handle_inputs(pkg);
        
        dbg(1, "Installing %s...", pkg->name);

        f_install_package_source(pkg->name, 0, repo);

        remove(pkg->name);

        return 0;
    }


    // install from repo without checking for the checksum
    int _install_repo_no_checksum_(unsigned int* i) {
        INSECURE = true;
        return 0;
    }

    int _set_debug_level_(unsigned int* i) {
        DEBUG = atoi(ARGV[++(*i)]);
        return 0;
    }
    int _set_debug_unit(unsigned int* i) {
        DEBUG_UNIT = ARGV[++(*i)];
        return 0;
    }
    int _set_verbose_(unsigned int* i) {
        QUIET = false;
        return 0;
    }
    int _set_overwrite_(unsigned int* i) {
        OVERWRITE = true;
        return 0;
    }
    // Create Bin from file function 
    int _create_binary_from_file(unsigned int* i) {
        char* file = ARGV[++(*i)];
        char* binary = ARGV[++(*i)];

        create_binary_from_source(file,binary);

        return 0;
    }

    // update function 
    int _update_(unsigned int* i)
    {
        sync();
        update();
        return 0;
    }
    // upgrade function
    int _upgrade_(unsigned int* i)
    {
        upgrade();
        return 0;
    }


    int _set_yes_(unsigned int* i) {
        OVERWRITE_CHOISE = true;
        USER_CHOISE[0] = "Y";
        return 0;
    }

    int _set_no_(unsigned int* i) {
        OVERWRITE_CHOISE = true;
        USER_CHOISE[0] = "N";
        return 0;
    }

    void ask_to_preview_pkg(char* name)
    {
        char* str = calloc(2, sizeof(char));

        msg(INFO, "Do you want to view the source for %s before installing? y/N", name);
        if(OVERWRITE_CHOISE != true)
        {
            char* res = fgets(str, 2, stdin);

            if ( strchr(str, '\n') == NULL )
            {
                while ((getchar()) != '\n');
            }

            int j = 0;

            while (str[j] != '\n' && str[j] != '\0')
            {
                j++;
            }

            if (str[j] == '\n')
            {
                str[j] = '\0';
            }
        }
            else
            {
                if(sizeof(USER_CHOISE[0]) == sizeof(str))
                {
                    sprintf(str, USER_CHOISE[0]);
                }
                    else
                    {
                        msg(FATAL, "something somwhere went wrong");
                    }
            }
        if((strcmp(str, "Y") == 0 || strcmp(str, "y") == 0))
        {
            char* cmd = calloc(MAX_PATH, sizeof(char));
            sprintf(cmd, "cat %s", name);
            char* output = exec(cmd);

            printf(output);

            char* str_2 = calloc(2, sizeof(char));

            msg(INFO, "Press q to abort the installation, hit enter to continue");
            if(OVERWRITE_CHOISE != true)
            {
                char* res_2 = fgets(str_2, 2, stdin);

                if ( strchr(str_2, '\n') == NULL )
                {
                    while ((getchar()) != '\n');
                }

                int k = 0;

                while (str_2[k] != '\n' && str_2[k] != '\0')
                {
                    k++;
                }

                if (str_2[k] == '\n')
                {
                    str_2[k] = '\0';
                }
            }
                else
                {
                    if(sizeof(USER_CHOISE[0]) == sizeof(str_2))
                    {
                        sprintf(str_2, USER_CHOISE[0]);
                    }
                        else
                        {
                            msg(FATAL, "something somwhere went wrong");
                        }
                }

            if((strcmp(str_2, "Q") == 0 || strcmp(str_2, "q") == 0))
            {
                remove(name);

                free(str);
                free(str_2);

                msg(FATAL, "Aborting...");
                return 0;
            }
                else
                {
                    msg(INFO, "Continuing...");
                }

            free(str_2);
        }
            else
            {
                msg(INFO, "Continuing...");
            }
        free(str);

    }

    void handle_inputs(struct package* pkg)
    {
        if(pkg->inputsCount > 0)
        {
            for (int i = 0; i < pkg->inputsCount; i++) 
                {
                    msg(INFO, "%s", pkg->inputs[i]);
                    char* str = calloc(MAX_PATH, sizeof(char));

                    if(!OVERWRITE_CHOISE)
                    {
                        char* res = fgets(str, MAX_PATH-1, stdin);
                        dbg(1, "Checking if enter was pressed");
                        if ( strchr(str, '\n') == NULL )
                        {
                            while ((getchar()) != '\n');
                        }

                        int k = 0;

                        while (str[k] != '\n' && str[k] != '\0')
                        {
                            dbg(1, "Checking if input %c is bad", str[k]);

                            if(str[k] == '~' 
                            | str[k] == '`' 
                            | str[k] == '#' 
                            | str[k] == '$' 
                            | str[k] == '&' 
                            | str[k] == '*' 
                            | str[k] == '(' 
                            | str[k] == ')' 
                            | str[k] == '\\'  
                            | str[k] == '|' 
                            | str[k] == '[' 
                            | str[k] == ']' 
                            | str[k] == '{'
                            | str[k] == '}' 
                            | str[k] == '\'' 
                            | str[k] == ';' 
                            | str[k] == '\\' 
                            | str[k] == '<' 
                            | str[k] == '>' 
                            | str[k] == '?' 
                            | str[k] == '!')
                            {
                                str[k] = ' ';
                            }
                            k++;
                        }

                        dbg(1, "replacing last new line");


                        if (str[k] == '\n')
                        {
                            str[k] = '\0';
                        }

                        char* in = calloc(128, sizeof(char));
                        sprintf(in, "INPUT_%d", i);
                        setenv(in, str, 0);
                        free(in);
                    }
                        else
                        {
                            sprintf(str, "%s", USER_CHOISE[0]);
                            char* in = calloc(128, sizeof(char));
                            sprintf(in, "INPUT_%d", i);
                            setenv(in, str, 0);
                            free(in);
                        }
                        free(str);
                }
        }    
    }
*/