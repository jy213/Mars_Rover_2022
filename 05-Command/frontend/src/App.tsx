import { useState } from 'react';
import Intro from './components/Intro';
import Welcoming_Page from './components/Welcoming_Page';
import Main_Dashboard from './components/Main_Dashboard';
import { BrowserRouter as Router, Routes, Route} from 'react-router-dom';

export default function App(){ 

  const [status, setStatus] = useState(false);
  return(
    <Router>
      <Routes>
        <Route path="/" element={<Intro/>}/>
        <Route path="/Login" element={<Welcoming_Page setStatus={setStatus}/>}/>
        <Route path="/Main_Dashboard" element={<Main_Dashboard/>}/>
      </Routes>
    </Router>
  )
}