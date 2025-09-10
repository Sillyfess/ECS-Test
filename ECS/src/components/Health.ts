import { Component } from '../Component';

export class Health extends Component {
    public current: number;
    public max: number;

    constructor(max = 100, current?: number) {
        super();
        this.max = max;
        this.current = current ?? max;
    }

    public takeDamage(amount: number): void {
        this.current = Math.max(0, this.current - amount);
    }

    public heal(amount: number): void {
        this.current = Math.min(this.max, this.current + amount);
    }

    public isDead(): boolean {
        return this.current <= 0;
    }

    public getPercentage(): number {
        return (this.current / this.max) * 100;
    }

    public clone(): Health {
        return new Health(this.max, this.current);
    }
}