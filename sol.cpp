
#include <bits/stdc++.h>
using namespace std;

class Task
{
private:
    int id;
    string description;
    string category;
    bool isCompleted;

public:
    // Constructor
    Task(int taskId, const string &desc, const string &cat)
        : id(taskId), description(desc), category(cat), isCompleted(false) {}

    // Default constructor for file loading
    Task() : id(0), description(""), category(""), isCompleted(false) {}

    // Getters
    int getId() const { return id; }
    string getDescription() const { return description; }
    string getCategory() const { return category; }
    bool getCompletionStatus() const { return isCompleted; }

    // Setters
    void setId(int taskId) { id = taskId; }
    void setDescription(const string &desc) { description = desc; }
    void setCategory(const string &cat) { category = cat; }
    void markCompleted() { isCompleted = true; }
    void markPending() { isCompleted = false; }

    // Display task information
    void displayTask() const
    {
        cout << "[" << id << "] "
             << (isCompleted ? "[✓] " : "[ ] ")
             << description << " (" << category << ")" << endl;
    }

    // Convert task to string for file saving
    string toString() const
    {
        return to_string(id) + "," + description + "," +
               category + "," + (isCompleted ? "1" : "0");
    }

    // Create task from file string
    void fromString(const string &taskData)
    {
        stringstream ss(taskData);
        string item;

        if (getline(ss, item, ','))
        {
            id = stoi(item);
        }
        if (getline(ss, item, ','))
        {
            description = item;
        }
        if (getline(ss, item, ','))
        {
            category = item;
        }
        if (getline(ss, item, ','))
        {
            isCompleted = (item == "1");
        }
    }
};

class TodoManager
{
private:
    vector<Task> tasks;
    int nextId;
    const string filename = "tasks.txt";

public:
    // Constructor
    TodoManager() : nextId(1)
    {
        loadTasksFromFile();
    }

    // Destructor - automatically save tasks
    ~TodoManager()
    {
        saveTasksToFile();
    }

    // Add new task
    void addTask()
    {
        string description, category;

        cout << "\n--- Add New Task ---" << endl;
        cout << "Enter task description: ";
        cin.ignore(); // Clear input buffer
        getline(cin, description);

        cout << "Enter category (work/personal/study/other): ";
        getline(cin, category);

        // Validate category
        if (category != "work" && category != "personal" &&
            category != "study" && category != "other")
        {
            category = "other";
            cout << "Invalid category. Set to 'other'." << endl;
        }

        Task newTask(nextId++, description, category);
        tasks.push_back(newTask);

        cout << "Task added successfully!" << endl;
    }

    // Mark task as completed
    void markTaskCompleted()
    {
        if (tasks.empty())
        {
            cout << "No tasks available!" << endl;
            return;
        }

        cout << "\n--- Mark Task as Completed ---" << endl;
        viewPendingTasks();

        int taskId;
        cout << "Enter task ID to mark as completed: ";
        cin >> taskId;

        bool found = false;
        for (auto &task : tasks)
        {
            if (task.getId() == taskId && !task.getCompletionStatus())
            {
                task.markCompleted();
                cout << "Task marked as completed!" << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "Task not found or already completed!" << endl;
        }
    }

    // View all pending tasks
    void viewPendingTasks()
    {
        cout << "\n--- Pending Tasks ---" << endl;
        bool hasPendingTasks = false;

        for (const auto &task : tasks)
        {
            if (!task.getCompletionStatus())
            {
                task.displayTask();
                hasPendingTasks = true;
            }
        }

        if (!hasPendingTasks)
        {
            cout << "No pending tasks!" << endl;
        }
    }

    // View all completed tasks
    void viewCompletedTasks()
    {
        cout << "\n--- Completed Tasks ---" << endl;
        bool hasCompletedTasks = false;

        for (const auto &task : tasks)
        {
            if (task.getCompletionStatus())
            {
                task.displayTask();
                hasCompletedTasks = true;
            }
        }

        if (!hasCompletedTasks)
        {
            cout << "No completed tasks!" << endl;
        }
    }

    // View tasks by category
    void viewTasksByCategory()
    {
        if (tasks.empty())
        {
            cout << "No tasks available!" << endl;
            return;
        }

        string category;
        cout << "\n--- View Tasks by Category ---" << endl;
        cout << "Enter category (work/personal/study/other): ";
        cin >> category;

        cout << "\n--- Tasks in '" << category << "' category ---" << endl;
        bool found = false;

        for (const auto &task : tasks)
        {
            if (task.getCategory() == category)
            {
                task.displayTask();
                found = true;
            }
        }

        if (!found)
        {
            cout << "No tasks found in this category!" << endl;
        }
    }

    // View all tasks
    void viewAllTasks()
    {
        if (tasks.empty())
        {
            cout << "No tasks available!" << endl;
            return;
        }

        cout << "\n--- All Tasks ---" << endl;
        for (const auto &task : tasks)
        {
            task.displayTask();
        }
    }

    // Delete a task
    void deleteTask()
    {
        if (tasks.empty())
        {
            cout << "No tasks available!" << endl;
            return;
        }

        cout << "\n--- Delete Task ---" << endl;
        viewAllTasks();

        int taskId;
        cout << "Enter task ID to delete: ";
        cin >> taskId;

        for (auto it = tasks.begin(); it != tasks.end(); ++it)
        {
            if (it->getId() == taskId)
            {
                tasks.erase(it);
                cout << "Task deleted successfully!" << endl;
                return;
            }
        }

        cout << "Task not found!" << endl;
    }

    // Save tasks to file
    void saveTasksToFile()
    {
        ofstream file(filename);
        if (!file)
        {
            cout << "Error: Cannot open file for saving!" << endl;
            return;
        }

        for (const auto &task : tasks)
        {
            file << task.toString() << endl;
        }

        file.close();
    }

    // Load tasks from file
    void loadTasksFromFile()
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "No existing task file found. Starting fresh!" << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                Task task;
                task.fromString(line);
                tasks.push_back(task);

                // Update nextId to avoid conflicts
                if (task.getId() >= nextId)
                {
                    nextId = task.getId() + 1;
                }
            }
        }

        file.close();
        cout << "Loaded " << tasks.size() << " tasks from file." << endl;
    }

    // Display menu - now with conditional save/exit option
    void displayMenu()
    {
        cout << "\n========== TO-DO LIST MANAGER ==========" << endl;
        cout << "1. Add Task" << endl;

        // Only show other options if tasks exist
        if (!tasks.empty())
        {
            cout << "2. Mark Task as Completed" << endl;
            cout << "3. View Pending Tasks" << endl;
            cout << "4. View Completed Tasks" << endl;
            cout << "5. View All Tasks" << endl;
            cout << "6. View Tasks by Category" << endl;
            cout << "7. Delete Task" << endl;
            cout << "8. Save and Exit" << endl;
            cout << "=========================================" << endl;
            cout << "Choose an option (1-8): ";
        }
        else
        {
            cout << "=========================================" << endl;
            cout << "Add your first task to unlock more options!" << endl;
            cout << "Choose option 1 to get started: ";
        }
    }

    // Main application loop - FIXED
    void run()
    {
        int choice;

        while (true)
        {
            displayMenu();

            // Clear input buffer before reading choice
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }

            // If no tasks exist, only allow adding tasks
            if (tasks.empty() && choice != 1)
            {
                cout << "Please add a task first to access other features!" << endl;
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                continue;
            }

            switch (choice)
            {
            case 1:
                addTask();
                break;
            case 2:
                if (!tasks.empty())
                {
                    markTaskCompleted();
                }
                else
                {
                    cout << "Invalid choice! Please try again." << endl;
                }
                break;
            case 3:
                if (!tasks.empty())
                {
                    viewPendingTasks();
                }
                else
                {
                    cout << "Invalid choice! Please try again." << endl;
                }
                break;
            case 4:
                if (!tasks.empty())
                {
                    viewCompletedTasks();
                }
                else
                {
                    cout << "Invalid choice! Please try again." << endl;
                }
                break;
            case 5:
                if (!tasks.empty())
                {
                    viewAllTasks();
                }
                else
                {
                    cout << "Invalid choice! Please try again." << endl;
                }
                break;
            case 6:
                if (!tasks.empty())
                {
                    viewTasksByCategory();
                }
                else
                {
                    cout << "Invalid choice! Please try again." << endl;
                }
                break;
            case 7:
                if (!tasks.empty())
                {
                    deleteTask();
                }
                else
                {
                    cout << "Invalid choice! Please try again." << endl;
                }
                break;
            case 8:
                if (!tasks.empty())
                {
                    saveTasksToFile();
                    cout << "Tasks saved successfully! Goodbye!" << endl;
                    return;
                }
                else
                {
                    cout << "Invalid choice! Please try again." << endl;
                }
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
            }

            // Pause before showing menu again
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
};

int main()
{
    TodoManager manager;
    manager.run();
    return 0;
}