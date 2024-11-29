// src/router/index.js
import { createRouter, createWebHistory } from 'vue-router';
import UserLogin from '../components/Login.vue';
import UserRegister from '../components/Register.vue';
import StartPage from  '../components/StartPage.vue';
import Mainpage from '../components/Mainpage.vue';
import profilePage from '../components/Profile.vue';
import ListingsPage from '../components/Listings.vue';
import SearchResults from '../components/SearchResults.vue';
import ForumPage from '../components/ForumPage.vue';
const routes = [
  {path: '/', component: StartPage},
  { path: '/login', component: UserLogin },
  { path: '/register', component: UserRegister },
  {path: '/Mainpage', component: Mainpage},
  {path: '/profile', component: profilePage},
  {path: '/listings', component: ListingsPage},
  {path: '/Results', component: SearchResults},
  {path: '/forum', component: ForumPage}
];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

export default router;
