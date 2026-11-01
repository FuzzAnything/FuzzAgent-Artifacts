// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 mutex and database connection functions.
 * Targets:
 *  - sqlite3_mutex_alloc
 *  - sqlite3_mutex_free
 *  - sqlite3_mutex_enter
 *  - sqlite3_mutex_leave
 *  - sqlite3_mutex_try
 *  - sqlite3_db_mutex
 */

/* Helper to open a database connection for testing sqlite3_db_mutex */
static sqlite3 *open_db_for_fuzzing(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    const char *db_path = "./dummy_file";
    
    /* Write some data to the dummy file to ensure it exists */
    FILE *f = fopen(db_path, "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }

    /* Attempt to open the database */
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        sqlite3_close(db);
        db = NULL;
    }
    
    return db;
}

/* Helper to close a database connection */
static void close_db_for_fuzzing(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    int mode = Data[0] % 8;
    int idx = 1;
    int alloc_type;
    sqlite3_mutex *mutex = NULL;
    sqlite3_mutex *db_mutex = NULL;
    sqlite3 *db = NULL;
    int is_recursive = 0;
    int enter_count, leave_count;
    int i;
    int current_ref_count = 0;

    switch (mode) {
        case 0:
        case 1:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
        case 76:
        case 77:
        case 78:
        case 79:
        case 80:
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86:
        case 87:
        case 88:
        case 89:
        case 90:
        case 91:
        case 92:
        case 93:
        case 94:
        case 95:
        case 96:
        case 97:
        case 98:
        case 99:
        case 100:
        case 101:
        case 102:
        case 103:
        case 104:
        case 105:
        case 106:
        case 107:
        case 108:
        case 109:
        case 110:
        case 111:
        case 112:
        case 113:
        case 114:
        case 115:
        case 116:
        case 117:
        case 118:
        case 119:
        case 120:
        case 121:
        case 122:
        case 123:
        case 124:
        case 125:
        case 126:
        case 127:
        case 128:
        case 129:
        case 130:
        case 131:
        case 132:
        case 133:
        case 134:
        case 135:
        case 136:
        case 137:
        case 138:
        case 139:
        case 140:
        case 141:
        case 142:
        case 143:
        case 144:
        case 145:
        case 146:
        case 147:
        case 148:
        case 149:
        case 150:
        case 151:
        case 152:
        case 153:
        case 154:
        case 155:
        case 156:
        case 157:
        case 158:
        case 159:
        case 160:
        case 161:
        case 162:
        case 163:
        case 164:
        case 165:
        case 166:
        case 167:
        case 168:
        case 169:
        case 170:
        case 171:
        case 172:
        case 173:
        case 174:
        case 175:
        case 176:
        case 177:
        case 178:
        case 179:
        case 180:
        case 181:
        case 182:
        case 183:
        case 184:
        case 185:
        case 186:
        case 187:
        case 188:
        case 189:
        case 190:
        case 191:
        case 192:
        case 193:
        case 194:
        case 195:
        case 196:
        case 197:
        case 198:
        case 199:
        case 200:
        case 201:
        case 202:
        case 203:
        case 204:
        case 205:
        case 206:
        case 207:
        case 208:
        case 209:
        case 210:
        case 211:
        case 212:
        case 213:
        case 214:
        case 215:
        case 216:
        case 217:
        case 218:
        case 219:
        case 220:
        case 221:
        case 222:
        case 223:
        case 224:
        case 225:
        case 226:
        case 227:
        case 228:
        case 229:
        case 230:
        case 231:
        case 232:
        case 233:
        case 234:
        case 235:
        case 236:
        case 237:
        case 238:
        case 239:
        case 240:
        case 241:
        case 242:
        case 243:
        case 244:
        case 245:
        case 246:
        case 247:
        case 248:
        case 249:
        case 250:
        case 251:
        case 252:
        case 253:
        case 254:
        case 255:
            /* Dynamic mutex operations */
            if (idx < Size) {
                alloc_type = Data[idx] % 2;
                idx++;
            } else {
                alloc_type = 0;
            }

            if (alloc_type == 0) {
                mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
                is_recursive = 0;
            } else {
                mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_RECURSIVE);
                is_recursive = 1;
            }

            if (mutex) {
                current_ref_count = 0;

                /* Try to enter the mutex */
                if (idx < Size) {
                    enter_count = Data[idx] % 3;
                    idx++;
                } else {
                    enter_count = 1;
                }

                /* 
                 * For non-recursive mutexes, we can only enter once.
                 */
                if (!is_recursive) {
                    enter_count = 1;
                }

                for (i = 0; i < enter_count; i++) {
                    sqlite3_mutex_enter(mutex);
                    current_ref_count++;
                }

                /* 
                 * Only try-lock if the mutex is recursive.
                 */
                if (is_recursive) {
                    int try_count;
                    if (idx < Size) {
                        try_count = Data[idx] % 3;
                        idx++;
                    } else {
                        try_count = 1;
                    }

                    for (i = 0; i < try_count; i++) {
                        if (sqlite3_mutex_try(mutex) == SQLITE_OK) {
                            current_ref_count++;
                        }
                    }
                }

                /* Leave the mutex exactly the number of times we entered it */
                leave_count = current_ref_count;
                for (i = 0; i < leave_count; i++) {
                    sqlite3_mutex_leave(mutex);
                }

                /* Free the mutex */
                sqlite3_mutex_free(mutex);
            }
            break;

        case 2:
        case 3:
            /* Static mutex operations */
            if (idx < Size) {
                int static_id = Data[idx] % 14;
                idx++;
                
                int mapped_id;
                switch (static_id) {
                    case 0: mapped_id = SQLITE_MUTEX_STATIC_MAIN; break;
                    case 1: mapped_id = SQLITE_MUTEX_STATIC_MEM; break;
                    case 2: mapped_id = SQLITE_MUTEX_STATIC_OPEN; break;
                    case 3: mapped_id = SQLITE_MUTEX_STATIC_PRNG; break;
                    case 4: mapped_id = SQLITE_MUTEX_STATIC_LRU; break;
                    case 5: mapped_id = SQLITE_MUTEX_STATIC_PMEM; break;
                    case 6: mapped_id = SQLITE_MUTEX_STATIC_APP1; break;
                    case 7: mapped_id = SQLITE_MUTEX_STATIC_APP2; break;
                    case 8: mapped_id = SQLITE_MUTEX_STATIC_APP3; break;
                    case 9: mapped_id = SQLITE_MUTEX_STATIC_VFS1; break;
                    case 10: mapped_id = SQLITE_MUTEX_STATIC_VFS2; break;
                    case 11: mapped_id = SQLITE_MUTEX_STATIC_VFS3; break;
                    default: mapped_id = SQLITE_MUTEX_STATIC_MAIN; break;
                }

                mutex = sqlite3_mutex_alloc(mapped_id);
                
                if (mutex) {
                    if (idx < Size) {
                        enter_count = Data[idx] % 3;
                        idx++;
                    } else {
                        enter_count = 1;
                    }

                    /* Limit enter count to 1 for static mutexes to avoid recursion issues */
                    if (enter_count > 1) enter_count = 1;

                    for (i = 0; i < enter_count; i++) {
                        sqlite3_mutex_enter(mutex);
                    }

                    leave_count = enter_count;
                    for (i = 0; i < leave_count; i++) {
                        sqlite3_mutex_leave(mutex);
                    }
                    
                    /* Do NOT free static mutexes */
                }
            }
            break;

        case 4:
        case 5:
            /* Database connection mutex operations */
            db = open_db_for_fuzzing(Data, Size);
            
            if (db) {
                db_mutex = sqlite3_db_mutex(db);
                
                if (db_mutex) {
                    if (idx < Size) {
                        enter_count = Data[idx] % 3;
                        idx++;
                    } else {
                        enter_count = 1;
                    }
                    
                    /* Limit enter count to 1 for db mutex to avoid recursion issues */
                    if (enter_count > 1) enter_count = 1;

                    for (i = 0; i < enter_count; i++) {
                        sqlite3_mutex_enter(db_mutex);
                    }

                    leave_count = enter_count;
                    for (i = 0; i < leave_count; i++) {
                        sqlite3_mutex_leave(db_mutex);
                    }
                }
                
                close_db_for_fuzzing(db);
            }
            break;
    }

    return 0;
}