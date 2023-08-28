import './App.css';
import { useState } from "react"


function App() {
  const [todoList, setTodoList] = useState([]);
  const [newTask, setNewTask] = useState("");

  const handleChange = (event) => {
    setNewTask(event.target.value);
  }

  const addTask = () => {
    const newTodoList = [...todoList, newTask];
    setTodoList(newTodoList);
  }

  const deleteTask = (taskName) => {
    setTodoList(todoList.filter((task) => task !== taskName ));
  }

  return (
    <div className="App">
      <div className="addTask"> 
        <input onChange={handleChange}/>
        <button onClick={addTask}> Add Task </button>  
      </div>

      <div className="list"> 
        {todoList.map((task)=>{
          // For current element/item in map
          return (
            <div>
              <h1>{task}</h1>
              <button onClick={() => deleteTask(task)}>x</button>
            </div>
          );
        })}      
      </div>
    </div>
  );
}


export default App;
