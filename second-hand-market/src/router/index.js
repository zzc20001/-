// src/router/index.js
import { createRouter, createWebHistory } from 'vue-router';
import UserLogin from '../components/Login.vue';
import UserRegister from '../components/Register.vue';
import StartPage from  '../components/StartPage.vue';
const routes = [
  {path: '/', component: StartPage},
  { path: '/login', component: UserLogin },
  { path: '/register', component: UserRegister },
];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

export default router;
