<template>
  <div id="app">
    <header>
      <div class="branding">
        <img src="../assets/logo.png" alt="平台 Logo" class="logo" />
        <h1>校园二手物品交易平台</h1>
      </div>
      <nav>
        <router-link to="/register" class="nav-link">注册</router-link>
        <router-link to="/login" class="nav-link">登录</router-link>
      </nav>
    </header>
    <main>
      <div class="welcome-card">
        <h2>欢迎来到校园二手物品交易平台</h2>
        <p>在这里，您可以方便地买卖二手商品，快速找到所需物品。</p>
        <div class="cta-buttons">
          <router-link to="/register" class="btn primary">立即注册</router-link>
          <router-link to="/login" class="btn secondary">已有账号？登录</router-link>
        </div>
      </div>
    </main>
    <footer>
      <p>&copy; 2024 校园二手物品交易平台 | 由 <a href="https://example.com">校园开发团队</a> 提供技术支持</p>
    </footer>
  </div>
</template>

<script>
export default {
  name: "StartPage",
};
</script>

<style scoped>
/* 全局样式 */
#app {
  font-family: 'Arial', sans-serif;
  color: #333;
  margin: 0;
  display: flex;
  flex-direction: column;
  min-height: 100vh;
  background: linear-gradient(135deg, #6a82fb 0%, #fc5c7d 100%); /* 温暖渐变背景 */
  overflow-x: hidden;
}

/* 顶部样式 */
header {
  background-color: rgba(255, 255, 255, 0.85);
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1.5rem 2rem;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

.branding {
  display: flex;
  align-items: center;
  gap: 1.5rem;
}

.logo {
  width: 45px;
  height: 45px;
}

header h1 {
  font-size: 1.75rem;
  color: #2c3e50;
  font-weight: 600;
}

nav {
  display: flex;
  gap: 1.5rem;
}

.nav-link {
  color: #2c3e50;
  text-decoration: none;
  font-weight: bold;
  font-size: 1.1rem;
  transition: color 0.3s;
}

.nav-link:hover {
  color: #3498db;
}

/* 主内容样式 */
main {
  flex: 1;
  display: flex;
  justify-content: center;
  align-items: center;
  padding: 3rem;
}

.welcome-card {
  background-color: white;
  padding: 2.5rem;
  border-radius: 15px;
  box-shadow: 0 10px 20px rgba(0, 0, 0, 0.15);
  text-align: center;
  max-width: 550px;
  animation: fadeIn 1s ease-in-out;
}

.welcome-card h2 {
  margin: 0 0 1.5rem 0;
  color: #34495e;
  font-size: 2rem;
}

.welcome-card p {
  margin-bottom: 2rem;
  font-size: 1.2rem;
  color: #7f8c8d;
}

.cta-buttons {
  display: flex;
  gap: 1.5rem;
  justify-content: center;
}

.btn {
  padding: 0.85rem 1.6rem;
  border-radius: 30px;
  font-size: 1.1rem;
  font-weight: bold;
  text-decoration: none;
  transition: all 0.3s;
  cursor: pointer;
}

.btn.primary {
  background-color: #3498db;
  color: white;
  border: none;
  box-shadow: 0 4px 10px rgba(52, 152, 219, 0.2);
}

.btn.primary:hover {
  background-color: #2980b9;
}

.btn.secondary {
  background-color: white;
  color: #3498db;
  border: 2px solid #3498db;
  box-shadow: 0 4px 10px rgba(52, 152, 219, 0.2);
}

.btn.secondary:hover {
  background-color: #ecf4fb;
  color: #2980b9;
}

/* 底部样式 */
footer {
  text-align: center;
  padding: 1.5rem;
  background-color: rgba(255, 255, 255, 0.95);
  font-size: 0.9rem;
  color: #7f8c8d;
}

footer a {
  color: #3498db;
  text-decoration: none;
}

footer a:hover {
  text-decoration: underline;
}

/* 动画效果 */
@keyframes fadeIn {
  from {
    opacity: 0;
    transform: translateY(20px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}
</style>
