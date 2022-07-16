import React from 'react';
import './Intro.css';
import Box from '@mui/material/Box';
import AppBar from '@mui/material/AppBar';
import Toolbar from '@mui/material/Toolbar';
import Button from '@mui/material/Button';
import LoginIcon from '@mui/icons-material/Login';
import video1 from './assets/video1.mp4';
import { useNavigate } from 'react-router-dom';


export default function Intro() {
    
    let navigate = useNavigate();

    const direct = async () => (
        navigate("/Login")
    )

  return (
    <div className='main'>
        <video src={video1} autoPlay loop/>
    <Box>
        <AppBar sx={{background: 'none', color:'#fff'}} elevation={0}>
            <Toolbar sx={{justifyContent: 'space-between', width: '80%', margin: '0 auto'}}>
                <h1>Team07</h1> 
                <Button variant="text" endIcon={<LoginIcon />} onClick={direct} sx={{fontSize: '2rem', fontFamily: "Nunito", color:'#fff'}} >
                    Enter
                </Button>
            </Toolbar>
          </AppBar>
    </Box>
    </div>
  )
}
