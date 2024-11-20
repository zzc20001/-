import { createApp } from 'vue';
import App from './App.vue';
import router from './router'; // 如果使用 Vue Router

const app = createApp(App);

app.use(router); // 如果有路由

app.mount('#app');
