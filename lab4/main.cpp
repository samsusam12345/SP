#include <stdio.h>
#include <fstream>
#include <string>
#include <pthread.h>
#include <mutex>
#include <vector>
#include <list>
#include <unistd.h>

std::mutex stack_mtx;
std::mutex count_mtx;

bool is_number(char* number)
{
    int size = strlen(number);
    for (int i = 0; i < size; i++)
        if (!isdigit(number[0]))
            return false;
    return true;
}

struct read_file_args
{
    std::string path;
    std::list<std::string> *text_stack;
};

struct substr_count_args
{
    std::list<std::string> *text_stack;
    std::string substr;
    int *count;
};

void *read_file(void* path)
{
    struct read_file_args *args = (struct read_file_args*)path;
    std::ifstream file_stream;
    file_stream.open(args->path, std::ifstream::in);
    if (file_stream.is_open())
    {
        std::string line;
        while (std::getline(file_stream, line))
        {
            if (line != "\r")
            stack_mtx.lock();
            args->text_stack->push_front(line);
            stack_mtx.unlock();
        }
    }
}

void *substr_count_th(void* sc_args)
{
    struct substr_count_args *args = (struct substr_count_args*)sc_args;
    std::printf("Thread %lu has started working.\n", pthread_self());
    std::string substr = args->substr;
    std::string line;
    while (true)
    {
        //std::printf("Thread %lu: stack size - %i.\n", pthread_self(), args->text_stack->size());
        stack_mtx.lock();
        if (args->text_stack->size() > 0)
        {
        line = args->text_stack->front();
        args->text_stack->pop_front();
        stack_mtx.unlock();
        }
        else
        {
            stack_mtx.unlock();
            break;
        }
        

        int str_size = strlen(line.c_str());
        int substr_size = strlen(substr.c_str());
        int local_count = 0;
        for (int line_index = 0; line_index < str_size - substr_size; line_index++)
        {
            int substr_index = 0;
            while (substr[substr_index] == line[line_index + substr_index])
            {
                substr_index++;
            }
            if (substr_index == substr_size)
            {
                substr_index = 0;
                local_count++;
            }
        }
        if (local_count > 0)
        {
            count_mtx.lock();
            *(args->count) += local_count;
            count_mtx.unlock();
        }
    }
    std::printf("Thread %lu has finished working.\n", pthread_self());
}

int main(int argc, char **argv)
{
    pid_t process_id;
    int exit_code;
    int count = 0;
    std::string file_path;
    std::string substr;
    int max_threads = -1;

    for (int i = 0; i < argc; i++)
    {
        if (i == 1)
        {
            if (strlen(argv[1]) < 1)
            {
                std::printf("Enter a file path to search in: ");
                std::scanf("%s", file_path.c_str());
            }
            else
            {
                file_path = argv[1];
            }  
        }
        if (i == 2)
        {
            if (strlen(argv[2]) < 1)
            {
                std::printf("Enter a string to search: ");
                std::scanf("%s", substr.c_str());
            }
            else
            {
                substr = argv[2];
            }
        }
        if (i == 3)
        {
            if (is_number(argv[3]))
            {
                max_threads = atoi(argv[3]);
            }
            while (!is_number(argv[3]) || max_threads < 1)
            {
                std::printf("Enter the number of threads to use while searching: ");
                std::scanf("%i", max_threads);
            }
        }
    }

    if (file_path.empty())
    {
        std::printf("Enter a file path to search in: ");
        std::scanf("%s", file_path.c_str());
    }
    if (substr.empty())
    {
        std::printf("Enter a string to search: ");
        std::scanf("%s", substr.c_str());
    }
    while (max_threads < 1)
    {
        std::printf("Enter the number of threads to use while searching: ");
        std::scanf("%i", &max_threads);
    }

    process_id=fork();
    if (process_id == -1)
    {
            perror("fork");
            exit(1);
    }
    else if (process_id == 0)
    {
    printf("CHILD: This is a child process!\n");
    printf("CHILD: My PID is %d\n", getpid());
    printf("CHILD: My parent's PID is %d\n", getppid());
    
    std::list<std::string> text_stack;

    struct read_file_args rf_args;
    rf_args.path = file_path;
    rf_args.text_stack = &text_stack;
    pthread_t rf_thread;
    int status = pthread_create(&rf_thread, NULL, read_file, (void*)&rf_args);
    if (status != 0) perror ("pthread_create"), exit( EXIT_FAILURE);
    pthread_join(rf_thread, NULL);
    pthread_t threads[max_threads];
    struct substr_count_args sc_args;
    sc_args.count = &count;
    sc_args.text_stack = &text_stack;
    sc_args.substr = substr;
    for (int i = 0; i < max_threads; i++)
    {
        int status = pthread_create(&threads[i], NULL, substr_count_th, (void*)&sc_args);
        if (status != 0) perror ("pthread_create"), exit( EXIT_FAILURE);
    }
    for (int i = 0; i < max_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    std::printf("%s occured in text %i times\n", substr.c_str(), count);
    printf("CHILD: Exiting with code %i\n", 0);
    exit(0);
    }
    else
    {
        printf("PARENT: This is a parent process!\n");
        printf("PARENT: My PID is %d\n", getpid());
        printf("PARENT: My child's PID is %d\n", process_id);
        printf("PARENT: Waiting till child calls exit\n");
        wait(&exit_code);
        printf("PARENT: Child's exit code: %d\n", WEXITSTATUS(exit_code));
        printf("PARENT: Terminating...\n");
    }
    

}