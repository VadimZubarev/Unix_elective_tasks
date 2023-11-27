#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <dbus/dbus.h>

#include <ctime>

const char *const INTERFACE_NAME = "dbus307.monitoring.use_internet";
const char *const SERVER_BUS_NAME = "dbus307.monitoring.use_internet_server";
const char *const OBJECT_PATH_NAME = "/dbus307/monitoring/use_internet";\
const char *const METHOD_NAME = "get_internet_time_use";

DBusError dbus_error;
void print_dbus_error (char *str);

#define PLATFORM_SERVICE          "org.freedesktop.NetworkManager"
#define PLATFORM_PATH             "/org/freedesktop/NetworkManager"
#define PLATFORM_CONNECTION_IF    "org.freedesktop.NetworkManager"

unsigned long use_internet_period = 0;
unsigned long start_internet_time = 0;
bool internet_flag = false;

unsigned long get_start_state(DBusConnection* conn){
    DBusError err;
    DBusMessageIter args;

    dbus_error_init(&err);

    DBusMessage* dbus_msg = dbus_message_new_method_call(PLATFORM_SERVICE, PLATFORM_PATH, PLATFORM_CONNECTION_IF, "state");
    DBusMessage* dbus_reply = dbus_connection_send_with_reply_and_block(conn, dbus_msg, DBUS_TIMEOUT_USE_DEFAULT, &err);
    if (!dbus_message_iter_init(dbus_reply, &args))
        printf("dbus: Message Has No Parameters\n");
    else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)){
        printf("dbus: Argument is not uint_32t!\n");
    }
    else{
        unsigned long temp_data;
        dbus_message_iter_get_basic(&args, &temp_data);
        return temp_data;
    }
    return 0;
}

unsigned long get_state(DBusConnection* conn){
    DBusMessage* msg;
    DBusMessageIter args;

    dbus_connection_read_write_dispatch(conn,0);
    msg = dbus_connection_pop_message(conn);

    if (NULL == msg){
            sleep(1);
            return 0;
    }

    if (dbus_message_is_signal(msg, PLATFORM_CONNECTION_IF, "StateChanged")){
        if (!dbus_message_iter_init(msg, &args))
                printf("dbus: Message Has No Parameters\n");
        else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)){
                printf("dbus: Argument is not uint_32t!\n");
                printf("%d\n", dbus_message_iter_get_arg_type(&args));
        }
        else
        {
            unsigned long temp_data;
            dbus_message_iter_get_basic(&args, &temp_data);
            dbus_message_unref(msg);
            return temp_data;
        }
    }

    dbus_message_unref(msg);
    return 0;
}

void init_internet_checker(DBusError* err, DBusConnection* conn){
    if (dbus_error_is_set(err)){
        fprintf(stderr, "Connection Error (%s)n", err -> message);
        dbus_error_free(err);
    }

    if (NULL == conn){
        printf("Error in connection\n");
        exit(1);
    }
    dbus_bus_add_match(conn, "type='signal',interface='org.freedesktop.NetworkManager'", err);
    dbus_connection_flush(conn);

    if (dbus_error_is_set(err)){
        fprintf(stderr, "Match Error (%s)n", err -> message);
        exit(1);
    }
}

int main (int argc, char **argv)
{
    DBusConnection *conn;
    DBusConnection *conn_chacker;
    DBusError err;
    int ret;

    dbus_error_init (&dbus_error);
    conn = dbus_bus_get (DBUS_BUS_SESSION, &dbus_error);

    if (dbus_error_is_set (&dbus_error))
        print_dbus_error ("dbus_bus_get");

    if (!conn) 
        exit (1);


    dbus_error_init(&err);
    conn_chacker = dbus_bus_get(DBUS_BUS_SYSTEM, &err);

    init_internet_checker(&err, conn_chacker);

    printf("%d\n", get_start_state(conn_chacker)); // Получение изначального состояния
    
    if(get_start_state(conn_chacker) == 70){
        printf("true\n");
        internet_flag = true;
        start_internet_time = std::time(nullptr);
    }



    // Get a well known name
    ret = dbus_bus_request_name (conn, SERVER_BUS_NAME, DBUS_NAME_FLAG_DO_NOT_QUEUE, &dbus_error);

    if (dbus_error_is_set (&dbus_error))
        print_dbus_error ("dbus_bus_get");

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf (stderr, "Dbus: not primary owner, ret = %d\n", ret);
        exit (1);
    }

    // Handle request from clients
    while (1) {
        unsigned int cur_state = get_state(conn_chacker);
        if((cur_state == 60 || cur_state == 70) && (!internet_flag)){
            internet_flag = true;
            start_internet_time = std::time(nullptr);
        }
        if(cur_state != 60 && cur_state != 0 && cur_state != 70 && internet_flag){
            internet_flag = false;
            use_internet_period += std::time(nullptr) - start_internet_time;
            printf("time use: %d\n", use_internet_period);
        }

        // Block for msg from client
        if (!dbus_connection_read_write_dispatch (conn, -1)) {
            fprintf (stderr, "Not connected now.\n");
            // exit (1);
        }
     
        DBusMessage *message;

        if ((message = dbus_connection_pop_message (conn)) == NULL) {
            fprintf (stderr, "Did not get message\n");
            continue;
        } 
        
        if (dbus_message_is_method_call (message, INTERFACE_NAME, METHOD_NAME)) {
            DBusMessage *reply;
            // send reply
            unsigned long answer = use_internet_period;
            
            if(internet_flag){
                use_internet_period += std::time(nullptr) - start_internet_time;
                start_internet_time = std::time(nullptr);
                printf("time use: %d\n", use_internet_period);
                answer = use_internet_period;
            }
            if ((reply = dbus_message_new_method_return (message)) == NULL) {
                fprintf (stderr, "Error in dbus_message_new_method_return\n");
                exit (1);
            }

            if (!dbus_message_append_args (reply, DBUS_TYPE_UINT32, &answer)) {
                fprintf (stderr, "Error in dbus_message_iter_append_basic\n");
                exit (1);
            }

            if (!dbus_connection_send (conn, reply, NULL)) {
                fprintf (stderr, "Error in dbus_connection_send\n");
                exit (1);
            }

            dbus_connection_flush (conn);
            dbus_message_unref (reply);	
                
        }
    }

    return 0;
}


void print_dbus_error (char *str) 
{
    fprintf (stderr, "%s: %s\n", str, dbus_error.message);
    dbus_error_free (&dbus_error);
}
