import {useState,useEffect} from 'react'
import axios from 'axios'

const Locate_Rover  = () => {

    const [roverlocatex, setRoverlocateX] = useState([0]);
  
    const [roverlocatey, setRoverlocateY] = useState([0]);
    useEffect( ()=>{
        const intervalId = setInterval(() => {   
      const fetchData = async () => {
          try {
              const res = await axios.get('https://us-central1-rover-back-end.cloudfunctions.net/rover_1')
              //console.log("fetch original data from firebase: ", res)
              setRoverlocateX((res.data)[0])
              setRoverlocateY((res.data)[1])
          } 
          catch (error) {
              console.log(error)
          }
      }
      fetchData()
    }, 1000);
    return () => clearInterval(intervalId); 
  },  )

return [roverlocatex,roverlocatey];}

export default Locate_Rover