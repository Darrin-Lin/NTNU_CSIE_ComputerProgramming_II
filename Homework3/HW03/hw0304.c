#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>

#define _DEBUG_
#define DEBUG 1

#define HELP_MSG "\
  -p, --pid=dosbox-staging_pid       Give program the dosbox-staging's pid.\n\
  -a, --adress=game_memory_adress    Give program the game's memory adress(Hex, start with 0x).\n\
  -h, --help                         Show help message.\n\
Both -p and -m is necessory.\n\
"

#define GAME_MEM_SIZE (1 << 24)

#if defined(_DEBUG_)
#define debug(fmt, ...) \
    fprintf(stderr, "%s:%d:%s():\n" fmt, __FILE__, __LINE__, __func__)
#else
#define debug(fmt, ...) \
    {                   \
    }
#endif

enum option_index
{
    p_opt = 0,
    a_opt,
    h_opt,
};

struct _sCharacter_data
{
    uint8_t unused_0[8];
    uint8_t x;
    uint8_t y;
    uint8_t cha_img;
    uint8_t position;
    uint8_t run;
    uint8_t movement;
    uint8_t camp;
    uint8_t pic_id;

    uint16_t name;
    uint8_t item[8][2];
    // next 2
    uint8_t magic[5];
    uint8_t race_id;
    uint8_t job_id;
    uint8_t level;
    uint8_t ap_sta;
    uint8_t dp_sta;
    uint8_t hit_ev_sta;
    uint8_t poison;
    uint8_t paralysis;
    uint8_t no_magic;

    uint8_t unused_1[15];
    uint16_t power;
    // next 1
    uint16_t defence;
    uint8_t move_distance;
    uint8_t exp;
    uint8_t unused_2;
    uint16_t speed;
    uint16_t now_hp;
    uint16_t max_hp;
    uint16_t now_mp;
    uint16_t max_mp;

    uint16_t ap;
    uint16_t dp;
    uint16_t ht;
    uint16_t ev;
    int8_t unused_3[8];

} __attribute__((__packed__));
typedef struct _sCharacter_data sCharacter_data;

int8_t print_80_byte(int32_t file)
{
    char buffer[100] = {0};
    read(file, buffer, 80);
    for (int i = 0; i < 80; i++)
    {
        if (i % 16 == 0)
            fprintf(stderr, "\n");
        if (i % 16 == 8)
            fprintf(stderr, "| ");
        fprintf(stderr, "0x%hhx ", buffer[i]);
    }
    fprintf(stderr, "\n-----\n");
    return 0;
}

int main(int argc, char *argv[])
{
    struct option longopts[] = {
        {"pid", required_argument, NULL, 'p'},
        {"adress", required_argument, NULL, 'a'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};
    int8_t option[3] = {0};
    int32_t opt = 0;
    uint64_t adress_num = 0;
    char pid[512] = {0};
    while ((opt = getopt_long(argc, argv, "p:a:h", longopts, NULL)) != -1)
    {
        char *num_err = NULL;
        switch (opt)
        {
        case 'p':
            num_err = NULL;
            option[p_opt]++;
            if (option[p_opt] > 1)
            {
                fprintf(stdout, "Error: -p can only be used once.\n");
                goto err_arg;
            }
            int64_t num = strtol(optarg, &num_err, 10);
            if (*num_err != '\0')
            {
                fprintf(stdout, "Error: %s is not a decimal number.\n", optarg);
                goto err_arg;
            }
            if (num < 0)
            {
                fprintf(stdout, "Error: pid can't less than zero.");
            }
            strcpy(pid, optarg);
            break;

        case 'a':
            num_err = NULL;
            option[a_opt]++;
            if (option[a_opt] > 1)
            {
                fprintf(stdout, "Error: -p can only be used once.\n");
                goto err_arg;
            }
            if (strstr(optarg, "0x") != optarg)
            {
                fprintf(stdout, "Error: Please enter the hex start with 0x.\n");
                goto err_arg;
            }
            adress_num = strtol(optarg, &num_err, 16);
            if (*num_err != '\0')
            {
                fprintf(stdout, "Error: %s is not a hex number.\n", optarg);
                goto err_arg;
            }
            break;

        case 'h':
            fprintf(stdout, HELP_MSG);
            return 0;
            break;
        default:
            fprintf(stdout, "Error: Invalid argument.\n");
            goto err_arg;
            break;
        }
    }
    if (!(option[p_opt] && option[a_opt]))
    {
        fprintf(stdout, "Error: -p and -a is necessary.\n");
        goto err_arg;
    }
    int32_t game = 0;
    char path[1024] = {0};
    snprintf(path, 1024, "/proc/%s/mem", pid);
    game = open(path, O_RDWR);
    if (game == -1)
    {
        fprintf(stdout, "Can't open %s", path);
        goto err_arg;
    }
    // int64_t file_size = 0;
    // struct stat st;
    // if (fstat(game, &st) < 0)
    // {
    //     fprintf(stdout, "Error: state error\n");
    //     close(game);
    //     return -1;
    // }
    // perror("");
    // file_size = st.st_size;
    if (game == -1)
    {
        fprintf(stdout, "Can't open %s", path);
        goto err_arg;
    }

    // lseek(game, adress_num+11, SEEK_SET);
    uint16_t now_hp, now_mp, max_hp, max_mp, x, y;

    printf("main character's hp now:");
    scanf("%hu", &now_hp);
    printf("main character's max hp:");
    scanf("%hu", &max_hp);
    printf("main character's mp now:");
    scanf("%hu", &now_mp);
    printf("main character's max mp:");
    scanf("%hu", &max_mp);
    printf("main character's posion(x,y) (top-left is (0,0)):");
    scanf("%hu%hu", &x, &y);

    // printf("hp:%hu %hu %hu %hu\n",now_hp,now_mp,max_hp,max_mp);

    // adress_num+=11; // by try
    lseek(game, adress_num, SEEK_SET);
    printf("size: %d\n", _SC_PAGE_SIZE);
    char buffer[96] = {0};
    // 16mb = 1<<24
    printf("%lx\n", adress_num);
    printf("%d\n", game);

    int64_t idx = 0;
    uint8_t *memory = calloc(GAME_MEM_SIZE, sizeof(int8_t));
    if (memory == NULL)
    {
        printf("Can't allocate.\n");
        close(game);
        return -1;
    }
    if (read(game, memory, GAME_MEM_SIZE) <= 0)
    {
        printf("read error.\n");
        free(memory);
        close(game);
        return 0;
    }
    sCharacter_data characters_data[100];
    sCharacter_data tmp_character[10];
    int64_t offsets[10] = {0};
    int32_t data_count = 0;
    for (int32_t i = 72; i < (GAME_MEM_SIZE)-16; i++)
    {
        if (*((uint16_t *)(memory + i)) == now_hp && *((uint16_t *)(memory + i + 2)) == max_hp && *((uint16_t *)(memory + i + 4)) == now_mp && *((uint16_t *)(memory + i + 6)) == max_mp)
        {
            tmp_character[data_count] = *((sCharacter_data *)(memory - 72 + i));
            if (tmp_character[data_count].x == x && tmp_character[data_count].y == y)
            {
                offsets[data_count] = i - 72;
                data_count++;
            }
        }
    }
    if (DEBUG)
    {
        printf("data_caount: %d\n", data_count);
    }
    int8_t show_detail = 0;
    printf("Do you want to see the detail of each character? (1/0):");
    scanf("%hhd", &show_detail);

    for (int32_t i = 0; i < data_count; i++)
    {
        printf("%d.\n\n", i + 1);
        printf("offset:%ld\n", offsets[i]);
        if (!show_detail)
        {
            printf("x:%hhu,y:%hhu\npic_id:%hhu\n", tmp_character[i].x, tmp_character[i].y, tmp_character[i].pic_id);
            printf("level:%hhu\n", tmp_character[i].level);
        }
        // print each character's data
        if (DEBUG)
        {
            for (int32_t j = 0; j < sizeof(sCharacter_data); j++)
            {
                if (j % 16 == 0)
                    printf("\n");
                if (j % 16 == 8)
                    printf("| ");
                printf("0x%hhx ", *((uint8_t *)(&tmp_character[i]) + j));
            }
            printf("\n");
        }
        if (show_detail)
        {
            fprintf(stderr, "x:%hhu y:%hhu\n", tmp_character[i].x, tmp_character[i].y);
            fprintf(stderr, "cha_img:%hhu\n", tmp_character[i].cha_img);
            fprintf(stderr, "position:%hhu\n", tmp_character[i].position);
            fprintf(stderr, "run:%hhu\n", tmp_character[i].run);
            fprintf(stderr, "movement:%hhu\n", tmp_character[i].movement);
            fprintf(stderr, "camp:%hhu\n", tmp_character[i].camp);
            fprintf(stderr, "pic_id:%hhu\n", tmp_character[i].pic_id);
            fprintf(stderr, "name:%hu\n", tmp_character[i].name);
            fprintf(stderr, "item: ");
            for (int32_t j = 0; j < 8; j++)
            {
                fprintf(stderr, "%hhu ", tmp_character[i].item[j][1]);
            }
            fprintf(stderr, "\n");
            fprintf(stderr, "magic:");
            for (int32_t j = 0; j < 5; j++)
            {
                fprintf(stderr, "%hhu ", tmp_character[i].magic[j]);
            }
            fprintf(stderr, "\n");
            fprintf(stderr, "job_id:%hhu ", tmp_character[i].job_id);
            fprintf(stderr, "level:%hhu ", tmp_character[i].level);
            fprintf(stderr, "ap_sta:%hhu ", tmp_character[i].ap_sta);
            fprintf(stderr, "dp_sta:%hhu\n", tmp_character[i].dp_sta);
            fprintf(stderr, "hit_ev_sta:%hhu ", tmp_character[i].hit_ev_sta);
            fprintf(stderr, "poison:%hhu ", tmp_character[i].poison);
            fprintf(stderr, "paralysis:%hhu ", tmp_character[i].paralysis);
            fprintf(stderr, "no_magic:%hhu\n", tmp_character[i].no_magic);
            fprintf(stderr, "power:%hu ", tmp_character[i].power);
            fprintf(stderr, "defence:%hu ", tmp_character[i].defence);
            fprintf(stderr, "move_distance:%hhu", tmp_character[i].move_distance);
            fprintf(stderr, "exp:%hhu ", tmp_character[i].exp);
            fprintf(stderr, "speed:%hu\n", tmp_character[i].speed);
            fprintf(stderr, "now_hp:%hu", tmp_character[i].now_hp);
            fprintf(stderr, "max_hp:%hu ", tmp_character[i].max_hp);
            fprintf(stderr, "now_mp:%hu ", tmp_character[i].now_mp);
            fprintf(stderr, "max_mp:%hu\n", tmp_character[i].max_mp);
            fprintf(stderr, "ap:%hu ", tmp_character[i].ap);
            fprintf(stderr, "dp:%hu ", tmp_character[i].dp);
            fprintf(stderr, "ht:%hu ", tmp_character[i].ht);
            fprintf(stderr, "ev:%hu ", tmp_character[i].ev);
            fprintf(stderr, "speed:%hu\n", tmp_character[i].speed);
        }
        printf("--------------------------------------------------\n");
    }
    int64_t main_character_select = 0;
    while (1)
    {
        printf("Please choose which is the main character\n");

        scanf("%ld", &main_character_select);
        if (main_character_select > data_count || main_character_select < 1)
        {
            printf("Invalid input.\n");
        }
        else
        {
            main_character_select--;
            break;
        }
    }
    for (int32_t i = 0; i < 33; i++)
    {
        characters_data[i] = *((sCharacter_data *)(memory + offsets[main_character_select] + 96 * i));
    }
    for (int32_t i = 0; i < 33; i++)
    {
        printf("character %d\n", i + 1);
        printf("x:%hhu y:%hhu\n", characters_data[i].x, characters_data[i].y);
        printf("cha_img:%hhu\n", characters_data[i].cha_img);
    }
    printf("Do you want to see the detail of each character? (1/0):");
    scanf("%hhd", &show_detail);
    for (int32_t i = 0; i < 33; i++)
    {
        printf("%d.\n\n", i + 1);
        fprintf(stderr, "x:%hhu y:%hhu\n", characters_data[i].x, characters_data[i].y);
        fprintf(stderr, "cha_img:%hhu\n", characters_data[i].cha_img);
        if (show_detail)
        {
            fprintf(stderr, "position:%hhu\n", characters_data[i].position);
            fprintf(stderr, "run:%hhu\n", characters_data[i].run);
            fprintf(stderr, "movement:%hhu\n", characters_data[i].movement);
            fprintf(stderr, "camp:%hhu\n", characters_data[i].camp);
            fprintf(stderr, "pic_id:%hhu\n", characters_data[i].pic_id);
            fprintf(stderr, "name:%hu\n", characters_data[i].name);
            fprintf(stderr, "item: ");
            for (int32_t j = 0; j < 8; j++)
            {
                fprintf(stderr, "%hhu ", characters_data[i].item[j][1]);
            }
            fprintf(stderr, "\n");
            fprintf(stderr, "magic:");
            for (int32_t j = 0; j < 5; j++)
            {
                fprintf(stderr, "%hhu ", characters_data[i].magic[j]);
            }
            fprintf(stderr, "\n");
            fprintf(stderr, "job_id:%hhu ", characters_data[i].job_id);
            fprintf(stderr, "level:%hhu ", characters_data[i].level);
            fprintf(stderr, "ap_sta:%hhu ", characters_data[i].ap_sta);
            fprintf(stderr, "dp_sta:%hhu\n", characters_data[i].dp_sta);
            fprintf(stderr, "hit_ev_sta:%hhu ", characters_data[i].hit_ev_sta);
            fprintf(stderr, "poison:%hhu ", characters_data[i].poison);
            fprintf(stderr, "paralysis:%hhu ", characters_data[i].paralysis);
            fprintf(stderr, "no_magic:%hhu\n", characters_data[i].no_magic);
            fprintf(stderr, "power:%hu ", characters_data[i].power);
            fprintf(stderr, "defence:%hu ", characters_data[i].defence);
            fprintf(stderr, "move_distance:%hhu", characters_data[i].move_distance);
            fprintf(stderr, "exp:%hhu ", characters_data[i].exp);
            fprintf(stderr, "speed:%hu\n", characters_data[i].speed);
            fprintf(stderr, "now_hp:%hu", characters_data[i].now_hp);
            fprintf(stderr, "max_hp:%hu ", characters_data[i].max_hp);
            fprintf(stderr, "now_mp:%hu ", characters_data[i].now_mp);
            fprintf(stderr, "max_mp:%hu\n", characters_data[i].max_mp);
            fprintf(stderr, "ap:%hu ", characters_data[i].ap);
            fprintf(stderr, "dp:%hu ", characters_data[i].dp);
            fprintf(stderr, "ht:%hu ", characters_data[i].ht);
            fprintf(stderr, "ev:%hu ", characters_data[i].ev);
            fprintf(stderr, "speed:%hu\n", characters_data[i].speed);
        }
    }

    printf("Please choose the character you want to modify\n");
    int64_t character_select = 0;
    while (1)
    {
        scanf("%ld", &character_select);
        if (character_select > 33 || character_select < 1)
        {
            printf("Invalid input.\n");
        }
        else
        {
            character_select--;
            break;
        }
    }
    printf("Please enter the new value of the character\n");
    int8_t want_edit = 0;
    printf("1. HP now\n");
    printf("2. HP max\n");
    printf("3. MP now\n");
    printf("4. MP max\n");
    printf("5. MT\n");
    printf("6. DF\n");
    printf("7. MV\n");
    printf("8. EX\n");
    printf("9. DX\n");
    printf("10. item\n");

    scanf("%hhd", &want_edit);
    switch (want_edit)
    {
    case 1:
        printf("Please enter the new value of HP now\n");
        scanf("%hu", &characters_data[character_select].now_hp);
        break;
    case 2:
        printf("Please enter the new value of HP max\n");
        scanf("%hu", &characters_data[character_select].max_hp);
        break;
    case 3:
        printf("Please enter the new value of MP now\n");
        scanf("%hu", &characters_data[character_select].now_mp);
        break;
    case 4:
        printf("Please enter the new value of MP max\n");
        scanf("%hu", &characters_data[character_select].max_mp);
        break;
    case 5:
        printf("Please enter the new value of MT\n");
        scanf("%hu", &characters_data[character_select].power);
        break;
    case 6:
        printf("Please enter the new value of DF\n");
        scanf("%hu", &characters_data[character_select].defence);
        break;
    case 7:
        printf("Please enter the new value of MV\n");
        scanf("%hhu", &characters_data[character_select].move_distance);
        break;
    case 8:
        printf("Please enter the new value of EX\n");
        scanf("%hhu", &characters_data[character_select].exp);
        break;
    case 9:
        printf("Please enter the new value of DX\n");
        scanf("%hhu", &characters_data[character_select].speed);
        break;
    case 10:
        printf("Please choose which item you want to modify (1 ~ 8)\n");
        int8_t item_select = 0;
        while (1)
        {
            scanf("%hhd", &item_select);
            if (item_select > 8 || item_select < 1)
            {
                printf("Invalid input.\n");
            }
            else
            {
                item_select--;
                break;
            }
        }
        printf("Please enter the new value of item\n");
        scanf("%hhu", &characters_data[character_select].item[item_select][1]);
        printf("Please enter the new status of item (1. equip 2.have 3. no item\n");
        int8_t item_status = 0;
        while (1)
        {
            scanf("%hhd", &item_status);
            if (item_status > 3 || item_status < 1)
            {
                printf("Invalid input.\n");
            }
            else
            {
                switch (item_status)
                {
                case 1:
                    characters_data[character_select].item[item_select][0] = 0x40;
                    break;

                case 2:
                    characters_data[character_select].item[item_select][0] = 0x00;
                    break;
                case 3:
                    characters_data[character_select].item[item_select][0] = 0x80;
                    break;
                default:
                    break;
                }
                break;
            }
        }
        break;
    default:
        break;
    }
    lseek(game, adress_num + offsets[main_character_select], SEEK_SET);
    write(game, &characters_data[character_select], sizeof(sCharacter_data));

    free(memory);
    close(game);
    return 0;

err_arg:
    return -1;
err_close:
    close(game);
    return -1;
}