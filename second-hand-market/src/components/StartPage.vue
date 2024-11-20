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
    name: "MainPage",
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
    background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
    overflow-x: hidden;
  }
  
  /* 顶部样式 */
  header {
    background-color: rgba(255, 255, 255, 0.9);
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 1rem 2rem;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
  }
  
  .branding {
    display: flex;
    align-items: center;
    gap: 1rem;
  }
  
  .logo {
    width: 50px;
    height: 50px;
  }
  
  header h1 {
    font-size: 1.5rem;
    color: #007bff;
  }
  
  nav {
    display: flex;
    gap: 1.5rem;
  }
  
  .nav-link {
    color: #007bff;
    text-decoration: none;
    font-weight: bold;
    transition: color 0.3s;
  }
  
  .nav-link:hover {
    color: #0056b3;
  }
  
  /* 主内容样式 */
  main {
    flex: 1;
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 2rem;
  }
  
  .welcome-card {
    background-color: white;
    padding: 2rem;
    border-radius: 10px;
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.15);
    text-align: center;
    max-width: 500px;
    animation: fadeIn 1s ease-in-out;
  }
  
  .welcome-card h2 {
    margin: 0 0 1rem 0;
    color: #007bff;
  }
  
  .welcome-card p {
    margin-bottom: 1.5rem;
    font-size: 1rem;
    color: #555;
  }
  
  .cta-buttons {
    display: flex;
    gap: 1rem;
    justify-content: center;
  }
  
  .btn {
    padding: 0.75rem 1.5rem;
    border-radius: 5px;
    font-size: 1rem;
    font-weight: bold;
    text-decoration: none;
    transition: all 0.3s;
    cursor: pointer;
  }
  
  .btn.primary {
    background-color: #007bff;
    color: white;
    border: none;
  }
  
  .btn.primary:hover {
    background-color: #0056b3;
  }
  
  .btn.secondary {
    background-color: white;
    color: #007bff;
    border: 2px solid #007bff;
  }
  
  .btn.secondary:hover {
    background-color: #e6f2ff;
  }
  
  /* 底部样式 */
  footer {
    text-align: center;
    padding: 1rem;
    background-color: rgba(255, 255, 255, 0.9);
    font-size: 0.875rem;
    color: #555;
  }
  
  footer a {
    color: #007bff;
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
  